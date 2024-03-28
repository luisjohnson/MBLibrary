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
    std::shared_ptr<Modbus::ModbusDataArea> modbusDataArea = std::make_shared<Modbus::ModbusDataArea>();

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
            modbusDataArea->insertCoil(Modbus::Coil(i, i % 2 == 0));
            modbusDataArea->insertDiscreteInput(Modbus::DiscreteInput(i, i % 2 == 0));
            modbusDataArea->insertHoldingRegister(Modbus::HoldingRegister(i, i));
            modbusDataArea->insertInputRegister(Modbus::InputRegister(i, i));
        }
    }
};



// Coil Registers Tests
TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectData) {

    Modbus::ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x01});
    ASSERT_EQ(response[1], std::byte{0x02});
    ASSERT_EQ(response[2], std::byte{0b10101010});
    ASSERT_EQ(response[3], std::byte{0b00000010});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectDataForOddNumberOfCoils) {

    Modbus::ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x05}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x01});
    ASSERT_EQ(response[1], std::byte{0x01});
    ASSERT_EQ(response[2], std::byte{0b00001010});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectDataForSingleCoil) {

    Modbus::ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x01}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x01});
    ASSERT_EQ(response[1], std::byte{0x01});
    ASSERT_EQ(response[2], std::byte{0b00000000});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsExceptionForInvalidAddress) {

    Modbus::ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x0F}, std::byte{00}, std::byte{0x0A}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x81});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsExceptionForInvalidQuantity) {

    Modbus::ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0F}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x81});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                          std::make_shared<Modbus::ModbusDataArea>());

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x81});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadCoilsCorrectDataForMaxRegisters) {

    while (modbusDataArea->getAllCoils().size() < 2000) {
        int prevAddress = modbusDataArea->getAllCoils().back().getAddress();
        modbusDataArea->insertCoil(Modbus::Coil(prevAddress + 1, true));
    }

    Modbus::ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD0}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x01});
    ASSERT_EQ(response[1], std::byte{0xFA});
}

// Discrete Inputs Registers Tests
TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsCorrectData) {

    Modbus::ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x02});
    ASSERT_EQ(response[1], std::byte{0x02});
    ASSERT_EQ(response[2], std::byte{0b10101010});
    ASSERT_EQ(response[3], std::byte{0b00000010});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsCorrectDataForOddNumberOfDiscreteInputs) {

    Modbus::ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x05}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x02});
    ASSERT_EQ(response[1], std::byte{0x01});
    ASSERT_EQ(response[2], std::byte{0b00001010});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsCorrectDataForSingleCoil) {

    Modbus::ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x01}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x02});
    ASSERT_EQ(response[1], std::byte{0x01});
    ASSERT_EQ(response[2], std::byte{0b00000000});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForInvalidAddress) {

    Modbus::ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x0F}, std::byte{00}, std::byte{0x0A}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x82});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForInvalidQuantity) {

    Modbus::ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0F}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x82});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                          std::make_shared<Modbus::ModbusDataArea>());

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x82});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsCorrectDataForMaxRegisters) {

    while (modbusDataArea->getAllDiscreteInputs().size() < 2000) {
        int prevAddress = modbusDataArea->getAllDiscreteInputs().back().getAddress();
        modbusDataArea->insertDiscreteInput(Modbus::DiscreteInput(prevAddress + 1, true));
    }

    Modbus::ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD0}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x02});
    ASSERT_EQ(response[1], std::byte{0xFA});
}


TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForInvalidFunctionCode) {

    Modbus::ModbusPDU pdu({std::byte{0x2C}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
                          std::make_shared<Modbus::ModbusDataArea>());

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x80 + 0x2C});
    ASSERT_EQ(response[1], std::byte{0x01});
}

// Read Holding Registers Tests
TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsCorrectData) {

    Modbus::ModbusPDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
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

    Modbus::ModbusPDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x01}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x03});
    ASSERT_EQ(response[1], std::byte{0x02});
    ASSERT_EQ(response[2], std::byte{0x00});
    ASSERT_EQ(response[3], std::byte{0x01});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsExceptionForInvalidAddress) {

    Modbus::ModbusPDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x0F}, std::byte{00}, std::byte{0x0A}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x83});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsExceptionForInvalidQuantity) {

    Modbus::ModbusPDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0F}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x83});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::ModbusPDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                          std::make_shared<Modbus::ModbusDataArea>());

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x83});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersCorrectDataForMaxRegisters) {

    while (modbusDataArea->getAllHoldingRegisters().size() < 125) {
        int prevAddress = modbusDataArea->getAllHoldingRegisters().back().getAddress();
        modbusDataArea->insertHoldingRegister(Modbus::HoldingRegister(prevAddress + 1, prevAddress + 1));
    }

    Modbus::ModbusPDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{0x00}, std::byte{0x7D}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x03});
    ASSERT_EQ(response[1], std::byte{0xFA});
}

// Read Input Registers Tests
TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsCorrectData) {

    Modbus::ModbusPDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
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

    Modbus::ModbusPDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x01}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x04});
    ASSERT_EQ(response[1], std::byte{0x02});
    ASSERT_EQ(response[2], std::byte{0x00});
    ASSERT_EQ(response[3], std::byte{0x01});
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsExceptionForInvalidAddress) {

    Modbus::ModbusPDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x0F}, std::byte{00}, std::byte{0x0A}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x84});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsExceptionForInvalidQuantity) {

    Modbus::ModbusPDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0F}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x84});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::ModbusPDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                          std::make_shared<Modbus::ModbusDataArea>());

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x84});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadInputRegisterCorrectDataForMaxRegisters) {

    while (modbusDataArea->getAllInputRegisters().size() < 125) {
        int prevAddress = modbusDataArea->getAllInputRegisters().back().getAddress();
        modbusDataArea->insertInputRegister(Modbus::InputRegister(prevAddress + 1, prevAddress + 1));
    }

    Modbus::ModbusPDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{0x00}, std::byte{0x7D}},
                          modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x04});
    ASSERT_EQ(response[1], std::byte{0xFA});
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
