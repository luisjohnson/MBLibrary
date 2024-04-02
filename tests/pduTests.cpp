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
    Modbus::DataArea modbusDataAreaWithTenRegistersEach{}; // 10 coils, 10 discrete inputs, 10 holding registers, 10 input registers
    Modbus::DataArea modbusDataAreaWithMaxRegisters{}; // 2000 coils, 2000 discrete inputs, 123 holding registers, 123 input registers

    void SetUp() override {

        // Initialize the coils and discrete inputs
        for (int i = 0; i < 10; ++i) {
            coils.emplace_back(i, i % 2 == 0);
            discreteInputs.emplace_back(i, i % 2 == 0);
        }

        holdingRegisters = {Modbus::HoldingRegister(1, 0x1234), Modbus::HoldingRegister(2, 0x5678)};
        inputRegisters = {Modbus::InputRegister(1, 0x9ABC), Modbus::InputRegister(2, 0xDEF0)};

        // Initialize the modbus data areas with 10 registers each
        modbusDataAreaWithTenRegistersEach.generateCoils(0, 10, Modbus::ValueGenerationType::Ones);
        modbusDataAreaWithTenRegistersEach.generateDiscreteInputs(0, 10, Modbus::ValueGenerationType::Ones);
        modbusDataAreaWithTenRegistersEach.generateHoldingRegisters(0, 10, Modbus::ValueGenerationType::Ones);
        modbusDataAreaWithTenRegistersEach.generateInputRegisters(0, 10, Modbus::ValueGenerationType::Ones);

        // Generate the maximum number of registers
        modbusDataAreaWithMaxRegisters.generateCoils(0, Modbus::MAX_COILS,
                                                     Modbus::ValueGenerationType::Zeros);
        modbusDataAreaWithMaxRegisters.generateDiscreteInputs(0, Modbus::MAX_DISCRETE_INPUTS,
                                                              Modbus::ValueGenerationType::Zeros);
        modbusDataAreaWithMaxRegisters.generateHoldingRegisters(0, Modbus::MAX_HOLDING_REGISTERS,
                                                                Modbus::ValueGenerationType::Zeros);
        modbusDataAreaWithMaxRegisters.generateInputRegisters(0, Modbus::MAX_INPUT_REGISTERS,
                                                              Modbus::ValueGenerationType::Zeros);
    }
};


/*********************************************************************************************************************
\section  Read Coils Tests

    The following tests are for the Read Coils function code (0x01). The tests cover the following scenarios:
    -  The response returns the correct data when the quantity is a multiple of 8
    -  The response returns the correct data when the quantity is an odd number
    -  The response returns the correct data when the quantity is 1
    -  The response returns an exception when the address is invalid
    -  The response returns an exception when the quantity is invalid
    -  The response returns an exception when the range exceeds the maximum number of coils
    -  The response returns the correct data when the maximum number of coils is requested
    @see ReadCoilsResponseReturnsCorrectDataWithMultipleOfEight
    @see ReadCoilsResponseReturnsCorrectDataForOddNumberOfCoils
    @see ReadCoilsResponseReturnsCorrectDataForSingleCoil
    @see ReadCoilsResponseReturnsExceptionForInvalidAddress
    @see ReadCoilsResponseReturnsExceptionForInvalidQuantity
    @see ReadCoilsResponseReturnsExceptionForRangeExceedingMax
    @see ReadCoilsCorrectDataForMaxRegisters
 *********************************************************************************************************************/
TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectDataWithMultipleOfEight) {

    Modbus::PDU pdu({std::byte{0x01},
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{0x00}, // Quantity of coils MSB
                     std::byte{0x08}}, // Quantity of coils LSB; Quantity = 8
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x01}); // Function code: Read Coils (1)
    ASSERT_EQ(response[1], std::byte{0x01}); // One byte representing 8 coils
    ASSERT_EQ(response[2], std::byte{0b11111111}); // 8 coils with value 1
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectDataForOddNumberOfCoils) {

    Modbus::PDU pdu({std::byte{0x01}, // Function code: Read Coils (1)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{00}, // Quantity of coils MSB
                     std::byte{0x09}}, // Quantity of coils LSB; Quantity = 9
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x01}); // Function code: Read Coils (1)
    ASSERT_EQ(response[1], std::byte{0x02}); // Two bytes representing 9 coils
    ASSERT_EQ(response[2], std::byte{0b11111111}); // 8 coils with value 1
    ASSERT_EQ(response[3], std::byte{0b00000001}); // 1 coil with value 1, the rest are 0
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectDataForSingleCoil) {

    Modbus::PDU pdu({std::byte{0x01}, // Function code: Read Coils (1)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{00}, // Quantity of coils MSB
                     std::byte{0x01}}, // Quantity of coils LSB; Quantity = 1
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x01}); // Function code: Read Coils (1)
    ASSERT_EQ(response[1], std::byte{0x01}); // One byte representing 1 coil
    ASSERT_EQ(response[2], std::byte{0b00000001}); // 1 coil with value 1, the rest are 0
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsExceptionForInvalidAddress) {

    Modbus::PDU pdu({std::byte{0x01}, // Function code: Read Coils (1)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x0F}, // Starting address LSB; Address = 15 (invalid)
                     std::byte{00}, // Quantity of coils MSB
                     std::byte{0x0A}},// Quantity of coils LSB; Quantity = 10
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x81}); // Exception code: 0x80 + 0x01
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsExceptionForInvalidQuantity) {

    Modbus::PDU pdu({std::byte{0x01}, // Function code: Read Coils (1)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{0x00}, // Quantity of coils MSB
                     std::byte{0x0F}}, // Quantity of coils LSB; Quantity = 15 (invalid)
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x81}); // Exception code: 0x80 + 0x01
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::PDU pdu({std::byte{0x01}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x81});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadCoilsCorrectDataForMaxRegisters) {

    Modbus::PDU pdu({std::byte{0x01}, // Function code: Read Coils (1)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{0x07}, // Quantity of coils MSB
                     std::byte{0xD0}}, // Quantity of coils LSB; Quantity = 2000
                    modbusDataAreaWithMaxRegisters);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x01}); // Function code: Read Coils (1)
    ASSERT_EQ(response[1], std::byte{0xFA}); // 250 bytes representing 2000 coils
    // Verify all the data bytes are 0x00
    for (int i = 2; i < 252; ++i) {
        ASSERT_EQ(response[i], std::byte{0x00});
    }
}

/*********************************************************************************************************************
* Discrete Input Registers Tests
 * The following tests are for the Read Discrete Inputs function code (0x02). The tests cover the following scenarios:
 * -  The response returns the correct data when the quantity is a multiple of 8
 * -  The response returns the correct data when the quantity is an odd number
 * -  The response returns the correct data when the quantity is 1
 * -  The response returns an exception when the address is invalid
 * -  The response returns an exception when the quantity is invalid
 * -  The response returns an exception when the range exceeds the maximum number of discrete inputs
 * -  The response returns the correct data when the maximum number of discrete inputs is requested
 * @see ReadDiscreteInputsResponseReturnsCorrectDataWithMultipleOfEight
 * @see ReadDiscreteInputsResponseReturnsCorrectDataForOddNumberOfDiscreteInputs
 * @see ReadDiscreteInputsResponseReturnsCorrectDataForSingleCoil
 * @see ReadDiscreteInputsResponseReturnsExceptionForInvalidAddress
 * @see ReadDiscreteInputsResponseReturnsExceptionForInvalidQuantity
 * @see ReadDiscreteInputsResponseReturnsExceptionForRangeExceedingMax
 * @see ReadDiscreteInputsCorrectDataForMaxRegisters
 ********************************************************************************************************************/


TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsCorrectDataWithMultipleOfEight) {

    Modbus::PDU pdu({std::byte{0x02}, // Function code: Read Discrete Inputs (2)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{00}, // Quantity of discrete inputs MSB
                     std::byte{0x08}}, // Quantity of discrete inputs LSB; Quantity = 8
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x02}); // Function code: Read Discrete Inputs (2)
    ASSERT_EQ(response[1], std::byte{0x01}); // One byte representing 8 discrete inputs
    ASSERT_EQ(response[2], std::byte{0b11111111}); // 8 discrete inputs with value 1
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsCorrectDataForOddNumberOfDiscreteInputs) {

    Modbus::PDU pdu({std::byte{0x02},
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, //    Starting address LSB; Address = 1
                     std::byte{00},  // Quantity of discrete inputs MSB
                     std::byte{0x09}}, // Quantity of discrete inputs LSB; Quantity = 9
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x02}); // Function code: Read Discrete Inputs (2)
    ASSERT_EQ(response[1], std::byte{0x02}); // Two bytes representing 9 discrete inputs
    ASSERT_EQ(response[2], std::byte{0b11111111}); // 8 discrete inputs with value 1
    ASSERT_EQ(response[3], std::byte{0b00000001}); // 1 discrete input with value 1, the rest are 0
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsCorrectDataForSingleCoil) {

    Modbus::PDU pdu({std::byte{0x02}, // Function code: Read Discrete Inputs (2)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{0x00}, //  Quantity of discrete inputs MSB
                     std::byte{0x01}}, // Quantity of discrete inputs LSB; Quantity = 1
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 3);
    ASSERT_EQ(response[0], std::byte{0x02}); // Function code: Read Discrete Inputs (2)
    ASSERT_EQ(response[1], std::byte{0x01}); // One byte representing 1 discrete input
    ASSERT_EQ(response[2], std::byte{0b00000001}); // 1 discrete input with value 1, the rest are 0
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForInvalidAddress) {

    Modbus::PDU pdu({std::byte{0x02},// Function code: Read Discrete Inputs (2)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x0F}, // Starting address LSB; Address = 15 (invalid)
                     std::byte{0x00}, // Quantity of discrete inputs MSB
                     std::byte{0x0A}}, // Quantity of discrete inputs LSB; Quantity = 10
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x82}); // Exception code: 0x80 + 0x02
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForInvalidQuantity) {

    Modbus::PDU pdu({std::byte{0x02}, // Function code: Read Discrete Inputs (2)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{0x00}, // Quantity of discrete inputs MSB
                     std::byte{0x0F}}, // Quantity of discrete inputs LSB; Quantity = 15 (invalid)
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x82}); // Exception code: 0x80 + 0x02
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::PDU pdu({std::byte{0x02}, // Function code: Read Discrete Inputs (2)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{0x07},  // Quantity of discrete inputs MSB
                     std::byte{0xD1}}, // Quantity of discrete inputs LSB; Quantity = 2000 (invalid)
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2); // Exception code: 0x80 + 0x02
    ASSERT_EQ(response[0], std::byte{0x82}); // Exception code: 0x80 + 0x02
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, ReadDiscreteInputsCorrectDataForMaxRegisters) {

    Modbus::PDU pdu({std::byte{0x02}, // Function code: Read Discrete Inputs (2)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{0x07}, // Quantity of discrete inputs MSB
                     std::byte{0xD0}}, // Quantity of discrete inputs LSB; Quantity = 2000
                    modbusDataAreaWithMaxRegisters);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x02}); // Function code: Read Discrete Inputs (2)
    ASSERT_EQ(response[1], std::byte{0xFA}); // 250 bytes representing 2000 discrete inputs
    // Verify all the data bytes are 0x00
    for (int i = 2; i < 252; ++i) {
        ASSERT_EQ(response[i], std::byte{0x00});
    }
}


// *********************************************************************************************************************
// Read Holding Registers Tests
// *********************************************************************************************************************
TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsCorrectData) {

    Modbus::PDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0A}},
                    modbusDataAreaWithTenRegistersEach);

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
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x03});
    ASSERT_EQ(response[1], std::byte{0x02});
    ASSERT_EQ(response[2], std::byte{0x00});
    ASSERT_EQ(response[3], std::byte{0x01});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsExceptionForInvalidAddress) {

    Modbus::PDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x0F}, std::byte{00}, std::byte{0x0A}},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x83});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsExceptionForInvalidQuantity) {

    Modbus::PDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0F}},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x83});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::PDU pdu({std::byte{0x03}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x83});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersCorrectDataForMaxRegisters) {

    Modbus::PDU pdu({std::byte{0x03}, // Function code: Read Holding Registers (3)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{0x00}, // Quantity of registers MSB
                     static_cast<std::byte>(Modbus::MAX_HOLDING_REGISTERS)}, // Quantity of registers LSB; Quantity = 123
                    modbusDataAreaWithMaxRegisters);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 248);
    ASSERT_EQ(response[0], std::byte{0x03});
    ASSERT_EQ(response[1], std::byte{0xF6});
}

// Read Input Registers Tests
TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsCorrectData) {

    Modbus::PDU pdu({std::byte{0x04},
                     std::byte{0x00},
                     std::byte{0x01},
                     std::byte{00},
                     std::byte{0x0A}},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 22);
    ASSERT_EQ(response[0], std::byte{0x04});
    ASSERT_EQ(response[1], std::byte{0x14});
    ASSERT_EQ(response[2], std::byte{0x00});
    ASSERT_EQ(response[3], std::byte{0x00});
    ASSERT_EQ(response[4], std::byte{0x00});
    ASSERT_EQ(response[5], std::byte{0x00});
    ASSERT_EQ(response[6], std::byte{0x00});
    ASSERT_EQ(response[7], std::byte{0x00});
    ASSERT_EQ(response[8], std::byte{0x00});
    ASSERT_EQ(response[9], std::byte{0x00});
    ASSERT_EQ(response[10], std::byte{0x00});
    ASSERT_EQ(response[11], std::byte{0x00});
    ASSERT_EQ(response[12], std::byte{0x00});
    ASSERT_EQ(response[13], std::byte{0x00});
    ASSERT_EQ(response[14], std::byte{0x00});
    ASSERT_EQ(response[15], std::byte{0x00});
    ASSERT_EQ(response[16], std::byte{0x00});
    ASSERT_EQ(response[17], std::byte{0x00});
    ASSERT_EQ(response[18], std::byte{0x00});
    ASSERT_EQ(response[19], std::byte{0x00});
    ASSERT_EQ(response[20], std::byte{0x00});
    ASSERT_EQ(response[21], std::byte{0x00});
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsCorrectDataForSingleRegister) {

    Modbus::PDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x01}},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x04});
    ASSERT_EQ(response[1], std::byte{0x02});
    ASSERT_EQ(response[2], std::byte{0x00});
    ASSERT_EQ(response[3], std::byte{0x01});
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsExceptionForInvalidAddress) {

    Modbus::PDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x0F}, std::byte{00}, std::byte{0x0A}},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x84});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsExceptionForInvalidQuantity) {

    Modbus::PDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{00}, std::byte{0x0F}},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x84});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::PDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{0x07}, std::byte{0xD1}},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x84});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, ReadInputRegisterCorrectDataForMaxRegisters) {

    while (modbusDataAreaWithTenRegistersEach.getAllInputRegisters().size() < 125) {
        int prevAddress = modbusDataAreaWithTenRegistersEach.getAllInputRegisters().back().getAddress();
        modbusDataAreaWithTenRegistersEach.insertInputRegister(Modbus::InputRegister(prevAddress + 1, prevAddress + 1));
    }

    Modbus::PDU pdu({std::byte{0x04}, std::byte{0x00}, std::byte{0x01}, std::byte{0x00}, std::byte{0x7D}},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 252);
    ASSERT_EQ(response[0], std::byte{0x04});
    ASSERT_EQ(response[1], std::byte{0xFA});
}

TEST_F(ModbusPDUTest, WriteSingleCoilInvalidAddressResponse) {

    Modbus::PDU pdu({std::byte{0x05}, std::byte{0x00}, std::byte{0x0B}, std::byte{0xFF}, std::byte{0x00}},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x85});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, WriteSingleCoilInvalidValueResponse) {

    Modbus::PDU pdu({std::byte{0x05}, std::byte{0x00}, std::byte{0x01}, std::byte{0xAB}, std::byte{0xCD}},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x85});
    ASSERT_EQ(response[1], std::byte{0x03});
}

TEST_F(ModbusPDUTest, WriteSingleCoilsCorrectResponse) {

    auto lastCoil = modbusDataAreaWithTenRegistersEach.getAllCoils().back();

    int address = lastCoil.getAddress() + 1;
    modbusDataAreaWithTenRegistersEach.insertCoil(Modbus::Coil(address, false));
    auto valueMsb = std::byte{0xFF};
    auto valueLsb = std::byte{0x00};

    auto addressMsb = static_cast<std::byte>(address >> 8);
    auto addressLsb = static_cast<std::byte>(address & 0xFF);

    Modbus::PDU pdu({std::byte{0x05}, addressMsb, addressLsb, valueMsb, valueLsb},
                    modbusDataAreaWithTenRegistersEach);


    auto response = pdu.buildResponse();

    auto coil = modbusDataAreaWithTenRegistersEach.getCoils(address, address).front();

    ASSERT_TRUE(coil.read());
    ASSERT_EQ(response.size(), 5);
    ASSERT_EQ(response[0], std::byte{0x05});
    ASSERT_EQ(response[1], addressMsb);
    ASSERT_EQ(response[2], addressLsb);
    ASSERT_EQ(response[3], std::byte{0xFF});
    ASSERT_EQ(response[4], std::byte{0x00});

}

TEST_F(ModbusPDUTest, WriteSingleRegisterInvalidAddressResponse) {

    Modbus::PDU pdu({std::byte{0x06}, std::byte{0x00}, std::byte{0x0B}, std::byte{0xFF},
                     std::byte{0x00}}, modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x86});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, WriteSingleRegisterCorrectResponse) {

    auto lastRegister = modbusDataAreaWithTenRegistersEach.getAllHoldingRegisters().back();

    int address = lastRegister.getAddress() + 1;
    modbusDataAreaWithTenRegistersEach.insertHoldingRegister(Modbus::HoldingRegister(address, 0x0000));
    auto valueMsb = std::byte{0xAB};
    auto valueLsb = std::byte{0xCD};

    auto addressMsb = static_cast<std::byte>(address >> 8);
    auto addressLsb = static_cast<std::byte>(address & 0xFF);

    Modbus::PDU pdu({std::byte{0x06}, addressMsb, addressLsb, valueMsb, valueLsb},
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    auto reg = modbusDataAreaWithTenRegistersEach.getHoldingRegisters(address, 1).front();

    ASSERT_EQ(reg.read(), 0xABCD);
    ASSERT_EQ(response.size(), 5);
    ASSERT_EQ(response[0], std::byte{0x06});
    ASSERT_EQ(response[1], addressMsb);
    ASSERT_EQ(response[2], addressLsb);
    ASSERT_EQ(response[3], valueMsb);
    ASSERT_EQ(response[4], valueLsb);
}

TEST_F(ModbusPDUTest, WriteMultipleCoilsInvalidAddressResponse) {

    Modbus::PDU pdu({std::byte{0x0F}, // Function Code: Write Multiple Coils (15)
                     std::byte{0x00}, // Starting Address MSB
                     std::byte{0x0B}, // Starting Address LSB; Starting Address = 11
                     std::byte{0x00}, // Quantity of Outputs MSB
                     std::byte{0x02}, // Quantity of Outputs LSB; Quantity of Outputs = 2
                     std::byte{0x01}, // Byte Count = 1
                     std::byte{0b00000011}}, // Outputs Value
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x8F});
    ASSERT_EQ(response[1], std::byte{0x02});
}

TEST_F(ModbusPDUTest, WriteMultipleCoilsResponseInvalidQuantityResponse) {
    Modbus::DataArea dataArea;
    Modbus::PDU pdu({std::byte(0x0F), std::byte(0x00), std::byte(0x01), std::byte(0x07), std::byte(0xD1),
                     std::byte(0x03), std::byte(0xAC), std::byte(0xDB), std::byte(0x35)}, dataArea);

    auto response = pdu.buildResponse();
    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte(0x8F));
    ASSERT_EQ(response[1], std::byte(0x03));
}

TEST_F(ModbusPDUTest, WriteMultipleCoilsResponseInvalidByteCountResponse) {
    Modbus::DataArea dataArea;
    Modbus::PDU pdu({std::byte(0x0F), std::byte(0x00), std::byte(0x01), std::byte(0x07), std::byte(0xD0),
                     std::byte(0x03), std::byte(0xAC), std::byte(0xDB), std::byte(0x35)}, dataArea);

    auto response = pdu.buildResponse();
    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte(0x8F));
    ASSERT_EQ(response[1], std::byte(0x03));
}

TEST_F(ModbusPDUTest, WriteMultipleCoilsRigthQuantityNotEnoughDataToWriteResponse) {
    Modbus::DataArea dataArea;
    for (int i = 1; i < 2001; i++) {
        dataArea.insertCoil(Modbus::Coil(i, false));
    }

    std::vector<std::byte> rawData = {static_cast<std::byte>(Modbus::FunctionCode::WriteMultipleCoils),
                                      std::byte(0x00), std::byte(0x01), std::byte(0x07),
                                      std::byte(0xD0), std::byte(0xFA)};

    for (int i = 0; i < 125; i++) {
        rawData.push_back(std::byte(0xFF));
    }

    Modbus::PDU pdu(rawData, dataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte(0x8F));
    ASSERT_EQ(response[1], std::byte(0x03));
}

TEST_F(ModbusPDUTest, WriteMultipleCoilsCorrectResponse) {
    Modbus::DataArea dataArea;
    for (int i = 1; i < 2001; i++) {
        dataArea.insertCoil(Modbus::Coil(i, false));
    }

    std::vector<std::byte> rawData = {static_cast<std::byte>(Modbus::FunctionCode::WriteMultipleCoils),
                                      std::byte(0x00), std::byte(0x01), std::byte(0x07),
                                      std::byte(0xD0), std::byte(0xFA)};

    for (int i = 0; i < 250; i++) {
        rawData.push_back(std::byte(0xFF));
    }

    Modbus::PDU pdu(rawData, dataArea);

    auto response = pdu.buildResponse();

    for (auto coil: dataArea.getAllCoils()) {
        ASSERT_TRUE(coil.read());
    }

    ASSERT_EQ(response.size(), 5);
    ASSERT_EQ(response[0], std::byte(0x0F));
    ASSERT_EQ(response[1], std::byte(0x00));
    ASSERT_EQ(response[2], std::byte(0x01));
    ASSERT_EQ(response[3], std::byte(0x07));
    ASSERT_EQ(response[4], std::byte(0xD0));
}

TEST_F(ModbusPDUTest, WriteMultipleRegistersInvalidAddressResponse) {

    Modbus::PDU pdu({std::byte{0x10}, std::byte{0x00}, std::byte{0x0B}, std::byte{0x00},
                     std::byte{0x02}, std::byte{0x02}, std::byte{0xFF}, std::byte{0xFF}, std::byte{0xFF},
                     std::byte{0xFF}}, modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x90});
    ASSERT_EQ(response[1], std::byte{0x03});
}

TEST_F(ModbusPDUTest, WriteMultipleRegistersInvalidQuantityResponse) {

    // Modbus Area has 10 registers, so trying to write 11 registers should return an exception
    std::vector<std::byte> rawData = {std::byte{0x10}, std::byte{0x00}, std::byte{0x01}, std::byte{0x00},
                                      std::byte{0x0B}, std::byte{0x0B}};
    for (int i = 0; i < 12; i++) {
        rawData.push_back(std::byte{0xFF});
        rawData.push_back(std::byte{0xFF});
    }

    Modbus::PDU pdu(rawData, modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x90});
    ASSERT_EQ(response[1], std::byte{0x03});
}

TEST_F(ModbusPDUTest, WriteMultipleRegistersRigthQuantityNotEnoughDataToWriteResponse) {

    // Modbus Area has 10 registers, so trying to write 11 registers should return an exception
    std::vector<std::byte> rawData = {std::byte{0x10}, std::byte{0x00}, std::byte{0x01}, std::byte{0x00},
                                      std::byte{0x0A}, std::byte{0x0A}};
    for (int i = 0; i < 2; i++) {
        rawData.push_back(std::byte{0xFF});
        rawData.push_back(std::byte{0xFF});
    }

    Modbus::PDU pdu(rawData, modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x90});
    ASSERT_EQ(response[1], std::byte{0x03});
}

TEST_F(ModbusPDUTest, WriteMultipleRegistersCorrectResponse) {

    Modbus::DataArea dataArea;
    for (int i = 1; i < Modbus::MAX_HOLDING_REGISTERS + 1; i++) {
        dataArea.insertHoldingRegister(Modbus::HoldingRegister(i, 0));
    }

    std::vector<std::byte> rawData = {std::byte{0x10}, // Function Code: Write Multiple Registers (10)
                                      std::byte{0x00}, // Starting Address MSB
                                      std::byte{0x01}, // Starting Address LSB; Starting Address = 1
                                      std::byte{0x00}, // Quantity of Registers MSB
                                      std::byte{0x7B}, // Quantity of Registers LSB ; Quantity of Registers = 123
                                      std::byte{0xF6}}; // Byte Count = 123 * 2 = 246

    for (int i = 1; i < Modbus::MAX_HOLDING_REGISTERS + 1; i++) {
        rawData.push_back(std::byte{0xFF});
        rawData.push_back(std::byte{0xFF});
    }

    Modbus::PDU pdu(rawData, dataArea);

    auto response = pdu.buildResponse();

    for (auto reg: dataArea.getAllHoldingRegisters()) {
        ASSERT_EQ(reg.read(), 65535);
    }

    ASSERT_EQ(response.size(), 5);
    ASSERT_EQ(response[0], std::byte{0x10});
    ASSERT_EQ(response[1], std::byte{0x00});
    ASSERT_EQ(response[2], std::byte{0x01});
    ASSERT_EQ(response[3], std::byte{0x00});
    ASSERT_EQ(response[4], std::byte{0x7B});
}

TEST(ModbusTest, BytesToMBAPReturnsCorrectMBAPForValidBytes) {
    std::vector<std::byte> bytes = {std::byte(0x01), std::byte(0x02), std::byte(0x03), std::byte(0x04),
                                    std::byte(0x05), std::byte(0x06), std::byte(0x07)};
    Modbus::MBAP expectedMBAP{};
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
    std::vector<std::byte> expectedBytes = {std::byte(0x01), std::byte(0x02), std::byte(0x03),
                                            std::byte(0x04), std::byte(0x05), std::byte(0x06),
                                            std::byte(0x01)};

    std::vector<std::byte> actualBytes = Modbus::MBAPToBytes(mbap);

    EXPECT_EQ(expectedBytes, actualBytes);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
