#include <gtest/gtest.h>
#include <ModbusDataArea.h>
#include <Modbus.h>
#include <ModbusPDU.h>
#include <memory>



class ModbusPDUTest : public ::testing::Test {
protected:

    std::shared_ptr<ModbusDataArea> modbusDataArea = std::make_shared<ModbusDataArea>();

    void SetUp() override {
        for (int i = 1; i < 11; ++i) {
            auto coil = ModbusCoil(i, true);
            modbusDataArea->insertCoil(ModbusCoil(i, i % 2 == 0));
            modbusDataArea->insertDiscreteInput(ModbusDiscreteInput(i, i % 2 == 0));
        }
    }
};

// Coil Registers Tests
TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectData) {

    ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x01});
    ASSERT_EQ(response[1], std::byte{0x02});
    ASSERT_EQ(response[2], std::byte{0b10101010});
    ASSERT_EQ(response[3], std::byte{0b00000010});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectDataForOddNumberOfCoils) {

    ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x05}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x01});
    ASSERT_EQ(response[1], std::byte{0x01});
    ASSERT_EQ(response[2], std::byte{0b00001010});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectDataForSingleCoil) {

    ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x01}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x01});
    ASSERT_EQ(response[1], std::byte{0x01});
    ASSERT_EQ(response[2], std::byte{0b00000000});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsExceptionForInvalidAddress) {

    ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x0F}, std::byte{00}, std::byte{0x0A}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x81});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsExceptionForInvalidQuantity) {

    ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0F}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x81});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsExceptionForRangeExceedingMax) {

    ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                  std::make_shared<ModbusDataArea>());

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x81});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadCoilsCorrectDataForMaxRegisters) {

    while (modbusDataArea->getAllCoils().size() < 2000) {
        int prevAddress = modbusDataArea->getAllCoils().back().getAddress();
        modbusDataArea->insertCoil(ModbusCoil(prevAddress + 1, true));
    }

    ModbusPDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD0}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x01});
    ASSERT_EQ(response[1], std::byte{0xFA});
}

// Discrete Inputs Registers Tests

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsCorrectData) {

    ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x02});
    ASSERT_EQ(response[1], std::byte{0x02});
    ASSERT_EQ(response[2], std::byte{0b10101010});
    ASSERT_EQ(response[3], std::byte{0b00000010});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsCorrectDataForOddNumberOfDiscreteInputs) {

    ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x05}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x02});
    ASSERT_EQ(response[1], std::byte{0x01});
    ASSERT_EQ(response[2], std::byte{0b00001010});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsCorrectDataForSingleCoil) {

    ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x01}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x02});
    ASSERT_EQ(response[1], std::byte{0x01});
    ASSERT_EQ(response[2], std::byte{0b00000000});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForInvalidAddress) {

    ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x0F}, std::byte{00}, std::byte{0x0A}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x82});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForInvalidQuantity) {

    ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0F}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x82});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForRangeExceedingMax) {

    ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                  std::make_shared<ModbusDataArea>());

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x82});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsCorrectDataForMaxRegisters) {

    while (modbusDataArea->getAllDiscreteInputs().size() < 2000) {
        int prevAddress = modbusDataArea->getAllDiscreteInputs().back().getAddress();
        modbusDataArea->insertDiscreteInput(ModbusDiscreteInput(prevAddress + 1, true));
    }

    ModbusPDU pdu({std::byte{0x02}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD0}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x02});
    ASSERT_EQ(response[1], std::byte{0xFA});
}


TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForInvalidFunctionCode) {

    ModbusPDU pdu({std::byte{0x2C}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
                  std::make_shared<ModbusDataArea>());

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x80 + 0x2C});
    ASSERT_EQ(response[1], std::byte{0x01});
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
