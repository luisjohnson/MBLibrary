#include <gtest/gtest.h>
#include <ModbusDataArea.h>
#include <Modbus.h>
#include <ModbusPDU.h>
#include <memory>


class ModbusPDUTest : public ::testing::Test {
protected:
    std::vector<Modbus::Coil> coils;
    std::vector<Modbus::DiscreteInput> discreteInputs;
    std::vector<Modbus::HoldingRegister> holdingRegisters;
    std::vector<Modbus::InputRegister> inputRegisters;

    std::vector<Modbus::InputRegister> emptyInputRegisters{};
    std::vector<Modbus::HoldingRegister> emptyHoldingRegisters{};
    Modbus::DataArea modbusDataArea{};

    void SetUp() override {

        // Initialize the coils and discrete inputs
        for (int i = 0; i < 10; ++i) {
            coils.emplace_back(i, i % 2 == 0);
            discreteInputs.emplace_back(i, i % 2 == 0);
        }

        holdingRegisters = {Modbus::HoldingRegister(1, 0x1234), Modbus::HoldingRegister(2, 0x5678)};
        inputRegisters = {Modbus::InputRegister(1, 0x9ABC), Modbus::InputRegister(2, 0xDEF0)};


        for (int i = 1; i < 11; ++i) {
            auto coil = Modbus::Coil(i, true);
            modbusDataArea.insertCoil(Modbus::Coil(i, i % 2 == 0));
            modbusDataArea.insertDiscreteInput(Modbus::DiscreteInput(i, i % 2 == 0));
            modbusDataArea.insertHoldingRegister(Modbus::HoldingRegister(i, i));
            modbusDataArea.insertInputRegister(Modbus::InputRegister(i, i));
        }
    }
};



// Coil Registers Tests
TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectData) {

    Modbus::PDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x01});
    ASSERT_EQ(response[1], std::byte{0x02});
    ASSERT_EQ(response[2], std::byte{0b10101010});
    ASSERT_EQ(response[3], std::byte{0b00000010});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectDataForOddNumberOfCoils) {

    Modbus::PDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x05}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x01});
    ASSERT_EQ(response[1], std::byte{0x01});
    ASSERT_EQ(response[2], std::byte{0b00001010});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectDataForSingleCoil) {

    Modbus::PDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x01}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x01});
    ASSERT_EQ(response[1], std::byte{0x01});
    ASSERT_EQ(response[2], std::byte{0b00000000});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsExceptionForInvalidAddress) {

    Modbus::PDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x0F}, std::byte{00}, std::byte{0x0A}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x81});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsExceptionForInvalidQuantity) {

    Modbus::PDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0F}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x81});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::PDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x81});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadCoilsCorrectDataForMaxRegisters) {

    while (modbusDataArea.getAllCoils().size() < 2000) {
        int prevAddress = modbusDataArea.getAllCoils().back().getAddress();
        modbusDataArea.insertCoil(Modbus::Coil(prevAddress + 1, true));
    }

    Modbus::PDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD0}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x01});
    ASSERT_EQ(response[1], std::byte{0xFA});
}

// Discrete Inputs Registers Tests
TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsCorrectData) {

    Modbus::PDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x02});
    ASSERT_EQ(response[1], std::byte{0x02});
    ASSERT_EQ(response[2], std::byte{0b10101010});
    ASSERT_EQ(response[3], std::byte{0b00000010});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsCorrectDataForOddNumberOfDiscreteInputs) {

    Modbus::PDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x05}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x02});
    ASSERT_EQ(response[1], std::byte{0x01});
    ASSERT_EQ(response[2], std::byte{0b00001010});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsCorrectDataForSingleCoil) {

    Modbus::PDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x01}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x02});
    ASSERT_EQ(response[1], std::byte{0x01});
    ASSERT_EQ(response[2], std::byte{0b00000000});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForInvalidAddress) {

    Modbus::PDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x0F}, std::byte{00}, std::byte{0x0A}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x82});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForInvalidQuantity) {

    Modbus::PDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0F}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x82});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::PDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x82});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsCorrectDataForMaxRegisters) {

    while (modbusDataArea.getAllDiscreteInputs().size() < 2000) {
        int prevAddress = modbusDataArea.getAllDiscreteInputs().back().getAddress();
        modbusDataArea.insertDiscreteInput(Modbus::DiscreteInput(prevAddress + 1, true));
    }

    Modbus::PDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD0}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x02});
    ASSERT_EQ(response[1], std::byte{0xFA});
}


TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForInvalidFunctionCode) {

    Modbus::PDU pdu({std::byte{0x2C}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x80 + 0x2C});
    ASSERT_EQ(response[1], std::byte{0x01});
}

// Read Holding Registers Tests
TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsCorrectData) {

    Modbus::PDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 22);
    ASSERT_EQ(response[0], std::byte{0x03});
    ASSERT_EQ(response[1], std::byte{0x14});
    ASSERT_EQ(response[2], std::byte{0x00});
    ASSERT_EQ(response[3], std::byte{0x01});
    ASSERT_EQ(response[4], std::byte{0x00});
    ASSERT_EQ(response[5], std::byte{0x02});
    ASSERT_EQ(response[6], std::byte{0x00});
    ASSERT_EQ(response[7], std::byte{0x03});
    ASSERT_EQ(response[8], std::byte{0x00});
    ASSERT_EQ(response[9], std::byte{0x04});
    ASSERT_EQ(response[10], std::byte{0x00});
    ASSERT_EQ(response[11], std::byte{0x05});
    ASSERT_EQ(response[12], std::byte{0x00});
    ASSERT_EQ(response[13], std::byte{0x06});
    ASSERT_EQ(response[14], std::byte{0x00});
    ASSERT_EQ(response[15], std::byte{0x07});
    ASSERT_EQ(response[16], std::byte{0x00});
    ASSERT_EQ(response[17], std::byte{0x08});
    ASSERT_EQ(response[18], std::byte{0x00});
    ASSERT_EQ(response[19], std::byte{0x09});
    ASSERT_EQ(response[20], std::byte{0x00});
    ASSERT_EQ(response[21], std::byte{0x0A});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsCorrectDataForSingleRegister) {

    Modbus::PDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x01}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x03});
    ASSERT_EQ(response[1], std::byte{0x02});
    ASSERT_EQ(response[2], std::byte{0x00});
    ASSERT_EQ(response[3], std::byte{0x01});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsExceptionForInvalidAddress) {

    Modbus::PDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x0F}, std::byte{00}, std::byte{0x0A}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x83});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsExceptionForInvalidQuantity) {

    Modbus::PDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0F}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x83});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::PDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x83});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersCorrectDataForMaxRegisters) {

    while (modbusDataArea.getAllHoldingRegisters().size() < 125) {
        int prevAddress = modbusDataArea.getAllHoldingRegisters().back().getAddress();
        modbusDataArea.insertHoldingRegister(Modbus::HoldingRegister(prevAddress + 1, prevAddress + 1));
    }

    Modbus::PDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{0x00}, std::byte{0x7D}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x03});
    ASSERT_EQ(response[1], std::byte{0xFA});
}

// Read Input Registers Tests
TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsCorrectData) {

    Modbus::PDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 22);
    ASSERT_EQ(response[0], std::byte{0x04});
    ASSERT_EQ(response[1], std::byte{0x14});
    ASSERT_EQ(response[2], std::byte{0x00});
    ASSERT_EQ(response[3], std::byte{0x01});
    ASSERT_EQ(response[4], std::byte{0x00});
    ASSERT_EQ(response[5], std::byte{0x02});
    ASSERT_EQ(response[6], std::byte{0x00});
    ASSERT_EQ(response[7], std::byte{0x03});
    ASSERT_EQ(response[8], std::byte{0x00});
    ASSERT_EQ(response[9], std::byte{0x04});
    ASSERT_EQ(response[10], std::byte{0x00});
    ASSERT_EQ(response[11], std::byte{0x05});
    ASSERT_EQ(response[12], std::byte{0x00});
    ASSERT_EQ(response[13], std::byte{0x06});
    ASSERT_EQ(response[14], std::byte{0x00});
    ASSERT_EQ(response[15], std::byte{0x07});
    ASSERT_EQ(response[16], std::byte{0x00});
    ASSERT_EQ(response[17], std::byte{0x08});
    ASSERT_EQ(response[18], std::byte{0x00});
    ASSERT_EQ(response[19], std::byte{0x09});
    ASSERT_EQ(response[20], std::byte{0x00});
    ASSERT_EQ(response[21], std::byte{0x0A});
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsCorrectDataForSingleRegister) {

    Modbus::PDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x01}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x04});
    ASSERT_EQ(response[1], std::byte{0x02});
    ASSERT_EQ(response[2], std::byte{0x00});
    ASSERT_EQ(response[3], std::byte{0x01});
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsExceptionForInvalidAddress) {

    Modbus::PDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x0F}, std::byte{00}, std::byte{0x0A}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x84});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsExceptionForInvalidQuantity) {

    Modbus::PDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0F}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x84});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::PDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x84});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadInputRegisterCorrectDataForMaxRegisters) {

    while (modbusDataArea.getAllInputRegisters().size() < 125) {
        int prevAddress = modbusDataArea.getAllInputRegisters().back().getAddress();
        modbusDataArea.insertInputRegister(Modbus::InputRegister(prevAddress + 1, prevAddress + 1));
    }

    Modbus::PDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{0x00}, std::byte{0x7D}},
                    modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x04});
    ASSERT_EQ(response[1], std::byte{0xFA});
}

TEST_F(ModbusPDUTest, WriteSingleCoilsCorrectResponse) {

    int address = 1;

    auto coil = modbusDataArea.getCoils(address, 1).front();
    coil.write(false);
    ASSERT_FALSE(coil.read());
    auto valueMsb = std::byte{0xFF};
    auto valueLsb = std::byte{0x00};

    auto addressMsb = static_cast<std::byte>(address >> 8);
    auto addressLsb = static_cast<std::byte>(address & 0xFF);

    Modbus::PDU pdu({std::byte{0x05}, addressMsb, addressLsb, valueMsb, valueLsb},
                    modbusDataArea);


    auto response = pdu.buildResponse();

    ASSERT_TRUE(coil.read());
    ASSERT_EQ(response.size(), 5);
    ASSERT_EQ(response[0], std::byte{0x05});
    ASSERT_EQ(response[1], std::byte{0x00});
    ASSERT_EQ(response[2], std::byte{0x01});
    ASSERT_EQ(response[3], std::byte{0xFF});
    ASSERT_EQ(response[4], std::byte{0x00});

}

TEST(ModbusTest, BytesToMBAPReturnsCorrectMBAPForValidBytes) {
    std::vector<std::byte> bytes = {std::byte(0x01), std::byte(0x02), std::byte(0x03), std::byte(0x04), std::byte(0x05),
                                    std::byte(0x06), std::byte(0x07)};
    Modbus::MBAP expectedMBAP;
    expectedMBAP.transactionIdentifier = 0x0102;
    expectedMBAP.protocolIdentifier = 0x0304;
    expectedMBAP.length = 0x0506;
    expectedMBAP.unitIdentifier = 0x07;

    Modbus::MBAP actualMBAP = Modbus::bytesToMBAP(bytes);

    EXPECT_EQ(expectedMBAP.transactionIdentifier, actualMBAP.transactionIdentifier);
    EXPECT_EQ(expectedMBAP.protocolIdentifier, actualMBAP.protocolIdentifier);
    EXPECT_EQ(expectedMBAP.length, actualMBAP.length);
    EXPECT_EQ(expectedMBAP.unitIdentifier, actualMBAP.unitIdentifier);
}

TEST(ModbusTest, BytesToMBAPThrowsExceptionForInvalidBytes) {
    std::vector<std::byte> bytes = {std::byte(0x01), std::byte(0x02), std::byte(0x03)};

    EXPECT_THROW(Modbus::bytesToMBAP(bytes), std::invalid_argument);
}

TEST(ModbusTest, MBAPToBytesReturnsCorrectBytesForValidMBAP) {
    Modbus::MBAP mbap{};
    mbap.transactionIdentifier = 0x0102;
    mbap.protocolIdentifier = 0x0304;
    mbap.length = 0x0506;
    mbap.unitIdentifier = 0x01;
    std::vector<std::byte> expectedBytes = {std::byte(0x01), std::byte(0x02), std::byte(0x03), std::byte(0x04),
                                            std::byte(0x05), std::byte(0x06), std::byte(0x01)};

    std::vector<std::byte> actualBytes = Modbus::MBAPToBytes(mbap);

    EXPECT_EQ(expectedBytes, actualBytes);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
