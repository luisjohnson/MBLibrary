#include <gtest/gtest.h>
#include <ModbusDataArea.h>
#include <Modbus.h>
#include <ModbusPDU.h>
#include <memory>


class ModbusPDUTest : public ::testing::Test {
protected:
    Modbus::DataArea modbusDataAreaWithTenRegistersEach{}; // 10 coils, 10 discrete inputs, 10 holding registers, 10 input registers
    Modbus::DataArea modbusDataAreaWithMaxRegisters{}; // 2000 coils, 2000 discrete inputs, 123 holding registers, 123 input registers

    void SetUp() override {

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
                     std::byte{0x00}, // Starting address LSB; Address = 0
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
                     std::byte{0x00}, // Starting address LSB; Address = 1
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


/********************************************************************************************************************
// Read Holding Registers Tests
// The following tests are for the Read Holding Registers function code (0x03). The tests cover the following scenarios:
// - The response returns correct data
//  - The response returns correct data for a single register
// - The response returns an exception for an invalid address
// - The response returns an exception for an invalid quantity
//
// @see ReadHoldingRegistersResponseReturnsCorrectData
// @see ReadHoldingRegistersResponseReturnsCorrectDataForSingleRegister
// @see ReadHoldingRegistersResponseReturnsExceptionForInvalidAddress
// @see ReadHoldingRegistersResponseReturnsExceptionForInvalidQuantity
// @see ReadHoldingRegistersResponseReturnsExceptionForRangeExceedingMax
// @see ReadHoldingRegistersCorrectDataForMaxRegisters
// ********************************************************************************************************************/
TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsCorrectData) {

    Modbus::PDU pdu({std::byte{0x03},
                     std::byte{0x00},  // Starting address MSB
                     std::byte{0x00}, //  Starting address LSB; Address = 0
                     std::byte{0x00}, // Quantity of registers MSB
                     std::byte{0x0A}}, // Quantity of registers LSB; Quantity = 10
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 22);

    ASSERT_EQ(response[0], std::byte{0x03}); // Function code: Read Holding Registers (3)
    ASSERT_EQ(response[1], std::byte{0x14}); // 20 bytes representing 10 registers
    // Verify all the data registers are 0x01
    for (int i = 2; i < 22; i = i + 2) {
        ASSERT_EQ(response[i], std::byte{0x00});
        ASSERT_EQ(response[i + 1], std::byte{0x01});
    }
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsCorrectDataForSingleRegister) {

    Modbus::PDU pdu({std::byte{0x03}, // Function code: Read Holding Registers (3)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{0x00}, // Quantity of registers MSB
                     std::byte{0x01}}, // Quantity of registers LSB; Quantity = 1
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x03}); // Function code: Read Holding Registers (3)
    ASSERT_EQ(response[1], std::byte{0x02}); // 2 bytes representing 1 register
    ASSERT_EQ(response[2], std::byte{0x00}); // Register value: 0x00
    ASSERT_EQ(response[3], std::byte{0x01}); // Register value: 0x01
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsExceptionForInvalidAddress) {

    Modbus::PDU pdu({std::byte{0x03}, // Function code: Read Holding Registers (3)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x0F}, // Starting address LSB; Address = 15 (invalid)
                     std::byte{00}, //   Quantity of registers MSB
                     std::byte{0x0A}}, // Quantity of registers LSB; Quantity = 10
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x83}); // Exception code: 0x80 + 0x03
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsExceptionForInvalidQuantity) {

    Modbus::PDU pdu({std::byte{0x03}, // Function code: Read Holding Registers (3)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{00},  // Quantity of registers MSB
                     std::byte{0x0F}}, // Quantity of registers LSB; Quantity = 15 (invalid)
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x83}); // Exception code: 0x80 + 0x03
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::PDU pdu({std::byte{0x03}, //  Function code: Read Holding Registers (3)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{0x07},  // Quantity of registers MSB
                     std::byte{0xD1}}, // Quantity of registers LSB; Quantity = 2000 (invalid)
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x83}); // Exception code: 0x80 + 0x03
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, ReadHoldingRegistersCorrectDataForMaxRegisters) {

    Modbus::PDU pdu({std::byte{0x03}, // Function code: Read Holding Registers (3)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x00}, // Starting address LSB; Address = 0
                     std::byte{0x00}, // Quantity of registers MSB
                     static_cast<std::byte>(Modbus::MAX_HOLDING_REGISTERS)}, // Quantity of registers LSB; Quantity = 123
                    modbusDataAreaWithMaxRegisters);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 248);
    ASSERT_EQ(response[0], std::byte{0x03}); // Function code: Read Holding Registers (3)
    ASSERT_EQ(response[1], std::byte{0xF6}); // 246 bytes representing 123 registers
}

/********************************************************************************************************************
// Read Input Registers Tests
// The following tests are for the Read Input Registers function code (0x04). The tests cover the following scenarios:
// -  The response returns correct data
// -  The response returns correct data for a single register
 ********************************************************************************************************************/

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsCorrectData) {

    Modbus::PDU pdu({std::byte{0x04}, // Function code: Read Input Registers (4)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x00}, // Starting address LSB; Address = 0
                     std::byte{00}, // Quantity of registers MSB
                     std::byte{0x0A}}, // Quantity of registers LSB; Quantity = 10
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 22);
    ASSERT_EQ(response[0], std::byte{0x04}); // Function code: Read Input Registers (4)
    ASSERT_EQ(response[1], std::byte{0x14}); // 20 bytes representing 10 registers
    // Verify all the data registers are 0x01
    for (int i = 2; i < 22; i = i + 2) {
        ASSERT_EQ(response[i], std::byte{0x00});
        ASSERT_EQ(response[i + 1], std::byte{0x01});
    }
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsCorrectDataForSingleRegister) {

    Modbus::PDU pdu({std::byte{0x04}, // Function code: Read Input Registers (4)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x09}, // Starting address LSB; Address = 9
                     std::byte{00}, // Quantity of registers MSB
                     std::byte{0x01}}, // Quantity of registers LSB; Quantity = 1
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
    ASSERT_EQ(response[0], std::byte{0x04}); // Function code: Read Input Registers (4)
    ASSERT_EQ(response[1], std::byte{0x02}); // 2 bytes representing 1 register
    ASSERT_EQ(response[2], std::byte{0x00}); // Register value: 0x00
    ASSERT_EQ(response[3], std::byte{0x01}); // Register value: 0x01
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsExceptionForInvalidAddress) {

    Modbus::PDU pdu({std::byte{0x04},
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x0F}, // Starting address LSB; Address = 15 (invalid)
                     std::byte{00}, // Quantity of registers MSB
                     std::byte{0x0A}}, // Quantity of registers LSB; Quantity = 10
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x84}); // Exception code: 0x80 + 0x04
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsExceptionForInvalidQuantity) {

    Modbus::PDU pdu({std::byte{0x04}, // Function code: Read Input Registers (4)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{00},  // Quantity of registers MSB
                     std::byte{0x0F}}, // Quantity of registers LSB; Quantity = 15 (invalid)
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x84}); // Exception code: 0x80 + 0x04
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, ReadInputRegisterResponseReturnsExceptionForRangeExceedingMax) {

    Modbus::PDU pdu({std::byte{0x04}, // Function code: Read Input Registers (4)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x01}, // Starting address LSB; Address = 1
                     std::byte{0x07}, // Quantity of registers MSB
                     std::byte{0xD1}}, // Quantity of registers LSB; Quantity = 2000 (invalid)
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x84}); // Exception code: 0x80 + 0x04
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, ReadInputRegisterCorrectDataForMaxRegisters) {

    Modbus::PDU pdu({std::byte{0x04}, // Function code: Read Input Registers (4)
                     std::byte{0x00}, // Starting address MSB
                     std::byte{0x00}, // Starting address LSB; Address = 1
                     std::byte{0x00}, // Quantity of registers MSB
                     std::byte{0x7B}}, // Quantity of registers LSB; Quantity = 123
                    modbusDataAreaWithMaxRegisters);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 248);
    ASSERT_EQ(response[0], std::byte{0x04}); // Function code: Read Input Registers (4)
    ASSERT_EQ(response[1], std::byte{0xF6}); // 246 bytes representing 123 registers
}

/********************************************************************************************************************
// Write Single Coil Tests
// The following tests are for the Write Single Coil function code (0x05). The tests cover the following scenarios:
// -  The response returns an exception when the address is invalid
// -  The response returns an exception when the value is invalid
// -  The response returns the correct data when the coil is written
//
// @see WriteSingleCoilInvalidAddressResponse
// @see WriteSingleCoilInvalidValueResponse
// @see WriteSingleCoilsCorrectResponse
 ********************************************************************************************************************/
TEST_F(ModbusPDUTest, WriteSingleCoilInvalidAddressResponse) {

    Modbus::PDU pdu({std::byte{0x05}, // Function Code: Write Single Coil (5)
                     std::byte{0x00}, // Starting Address MSB
                     std::byte{0x0B},  // Starting Address LSB; Starting Address = 11 (invalid)
                     std::byte{0xFF}, // Output Value MSB
                     std::byte{0x00}}, // Output Value LSB; Output Value = 0xFF00
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x85}); // Exception code: 0x80 + 0x05
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, WriteSingleCoilInvalidValueResponse) {

    Modbus::PDU pdu({std::byte{0x05}, // Function Code: Write Single Coil (5)
                     std::byte{0x00}, // Starting Address MSB
                     std::byte{0x01},  // Starting Address LSB; Starting Address = 1
                     std::byte{0xAB},  // Output Value MSB
                     std::byte{0xCD}}, // Output Value LSB; Output Value = 0xABCD
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x85}); // Exception code: 0x80 + 0x05
    ASSERT_EQ(response[1], std::byte{0x03}); // Exception code: 0x03 (Illegal Data Value Exception)
}

TEST_F(ModbusPDUTest, WriteSingleCoilsCorrectResponse) {

    Modbus::PDU pdu({std::byte{0x05}, // Function Code: Write Single Coil (5)
                     std::byte{0x00}, // Starting Address MSB
                     std::byte{0x01},  // Starting Address LSB; Starting Address = 1
                     std::byte{0x00},  // Output Value MSB
                     std::byte{0x00}}, // Output Value LSB; Output Value = 0x0000 (Turn off coil)
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    auto coil = modbusDataAreaWithTenRegistersEach.getCoils(1, 1).front();

    ASSERT_EQ(response.size(), 5);
    ASSERT_EQ(response[0], std::byte{0x05}); // Function Code: Write Single Coil (5)
    ASSERT_EQ(response[1], std::byte{0x00}); // Starting Address MSB
    ASSERT_EQ(response[2], std::byte{0x01}); // Starting Address LSB
    ASSERT_EQ(response[3], std::byte{0x00}); // Output Value MSB
    ASSERT_EQ(response[4], std::byte{0x00}); // Output Value LSB
    ASSERT_EQ(coil.read(), false); // Coil value is 0
}

/********************************************************************************************************************
  Write Single Register Tests \\
  The following tests are for the Write Single Register function code (0x06). The tests cover the following scenarios: @n
    -  The response returns an exception when the address is invalid\\
    -  The response returns the correct data when the register is written\\

 @see WriteSingleRegisterInvalidAddressResponse
 @see WriteSingleRegisterCorrectResponse

 ********************************************************************************************************************/

TEST_F(ModbusPDUTest, WriteSingleRegisterInvalidAddressResponse) {

    Modbus::PDU pdu({std::byte{0x06}, // Function Code: Write Single Register (6)
                     std::byte{0x00}, // Starting Address MSB
                     std::byte{0x0B}, // Starting Address LSB; Starting Address = 11 (invalid)
                     std::byte{0xFF}, // Register Value MSB
                     std::byte{0x00}}, // Register Value LSB; Register Value = 0xFF00
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x86}); // Exception code: 0x80 + 0x06
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, WriteSingleRegisterCorrectResponse) {


    Modbus::PDU pdu({std::byte{0x06}, // Function Code: Write Single Register (6)
                     std::byte{0x00}, // Starting Address MSB
                     std::byte{0x01}, // Starting Address LSB; Starting Address = 1
                     std::byte{0xAB}, // Register Value MSB
                     std::byte{0xCD}}, // Register Value LSB; Register Value = 0xABCD (43981)
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    auto reg = modbusDataAreaWithTenRegistersEach.getHoldingRegisters(1, 1).front();

    ASSERT_EQ(response.size(), 5);
    ASSERT_EQ(response[0], std::byte{0x06}); // Function Code: Write Single Register (6)
    ASSERT_EQ(response[1], std::byte{0x00}); // Starting Address MSB
    ASSERT_EQ(response[2], std::byte{0x01}); // Starting Address LSB
    ASSERT_EQ(response[3], std::byte{0xAB}); // Register Value MSB
    ASSERT_EQ(response[4], std::byte{0xCD}); // Register Value LSB
    ASSERT_EQ(reg.read(), 43981); // Register value is 43981
}

/********************************************************************************************************************@n
   Write Multiple Coils Tests
    The following tests are for the Write Multiple Coils function code (0x0F). The tests cover the following scenarios:@n
    -  The response returns an exception when the address is invalid@n
    -  The response returns an exception when the quantity is invalid@n
    -  The response returns an exception when the byte count is invalid@n
    -  The response returns an exception when the range exceeds the maximum number of coils@n
    -  The response returns an exception when the data is not enough to write the coils@n
    -  The response returns the correct data when the coils are written@n

    @see WriteMultipleCoilsInvalidAddressResponse @n
    @see WriteMultipleCoilsResponseInvalidQuantityResponse @n
    @see WriteMultipleCoilsResponseInvalidByteCountResponse @n
    @see WriteMultipleCoilsRigthQuantityNotEnoughDataToWriteResponse @n
    @see WriteMultipleCoilsCorrectResponse @n


 ********************************************************************************************************************/

TEST_F(ModbusPDUTest, WriteMultipleCoilsInvalidAddressResponse) {
    // Write 2 coils starting from address 11
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
    ASSERT_EQ(response[0], std::byte{0x8F}); // Exception code: 0x80 + 0x0F
    ASSERT_EQ(response[1], std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception)
}

TEST_F(ModbusPDUTest, WriteMultipleCoilsResponseInvalidQuantityResponse) {
    // Write 100 coils starting from address 1 should return an exception response
    std::vector<std::byte> rawData = {std::byte(0x0F), // Function Code: Write Multiple Coils (15)
                                      std::byte(0x00), // Starting Address MSB
                                      std::byte(0x01), // Starting Address LSB; Starting Address = 1
                                      std::byte(0x00), // Quantity of Outputs MSB
                                      std::byte(0x64), // Quantity of Outputs LSB; Quantity of Outputs = 100 (invalid)
                                      std::byte(0xC8), // Byte Count = 200
                                      std::byte(0xAC), //
    };

    auto bytes = Modbus::calculateBytesFromBits(200);
    for (int i = 0; i < bytes; i++) {
        rawData.push_back(std::byte(0xFF));
    }
    Modbus::PDU pdu(rawData, modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte(0x8F)); // Exception code: 0x80 + 0x0F
    ASSERT_EQ(response[1], std::byte(0x03)); // Exception code: 0x03 (Illegal Data Value Exception)
}

TEST_F(ModbusPDUTest, WriteMultipleCoilsResponseInvalidByteCountResponse) {
    Modbus::PDU pdu({std::byte(0x0F), // Function Code: Write Multiple Coils (15)
                     std::byte(0x00),  // Starting Address MSB
                     std::byte(0x01), // Starting Address LSB; Starting Address = 1
                     std::byte(0x00), // Quantity of Outputs MSB
                     std::byte(0x08), // Quantity of Outputs LSB; Quantity of Outputs = 2000
                     std::byte(0x03), // Byte Count = 3 (invalid)
                     std::byte(0xFF)}, //  Outputs Value
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte(0x8F)); // Exception code: 0x80 + 0x0F
    ASSERT_EQ(response[1], std::byte(0x03)); // Exception code: 0x03 (Illegal Data Value Exception)
}

TEST_F(ModbusPDUTest, WriteMultipleCoilsRigthQuantityNotEnoughDataToWriteResponse) {
    // Write 200 coils starting from address 1 should return an exception response
    std::vector<std::byte> rawData = {std::byte(0x0F), // Function Code: Write Multiple Coils (15)
                                      std::byte(0x00), // Starting Address MSB
                                      std::byte(0x01), // Starting Address LSB; Starting Address = 1
                                      std::byte(0x07), // Quantity of Outputs MSB
                                      std::byte(0xD0), // Quantity of Outputs LSB; Quantity of Outputs = 2000
                                      std::byte(0xFA)}; // Byte Count = 250

    // Add only 125 bytes of data
    for (int i = 0; i < 125; i++) {
        rawData.push_back(std::byte(0xFF));
    }

    Modbus::PDU pdu(rawData, modbusDataAreaWithMaxRegisters);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte(0x8F)); // Exception code: 0x80 + 0x0F
    ASSERT_EQ(response[1], std::byte(0x03)); // Exception code: 0x03 (Illegal Data Value Exception)
}

TEST_F(ModbusPDUTest, WriteMultipleCoilsCorrectResponse) {

    // Write 7 coils starting from address 5
    std::vector<std::byte> rawData = {std::byte(0x0F), // Function Code: Write Multiple Coils (15)
                                      std::byte(0x00), // Starting Address MSB
                                      std::byte(0x03), // Starting Address LSB; Starting Address = 5
                                      std::byte(0x00), // Quantity of Outputs MSB
                                      std::byte(0x07), // Quantity of Outputs LSB; Quantity of Outputs = 7
                                      std::byte(0x01), // Byte Count = 1
                                      std::byte(0b01111111)}; // Outputs Value

    Modbus::PDU pdu(rawData, modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    for (auto coil: modbusDataAreaWithTenRegistersEach.getCoils(3, 7)) {
        ASSERT_TRUE(coil.read());
    }

    ASSERT_EQ(response.size(), 5);
    ASSERT_EQ(response[0], std::byte(0x0F)); // Function Code: Write Multiple Coils (15)
    ASSERT_EQ(response[1], std::byte(0x00)); // Starting Address MSB
    ASSERT_EQ(response[2], std::byte(0x03)); // Starting Address LSB
    ASSERT_EQ(response[3], std::byte(0x00)); // Quantity of Outputs MSB
    ASSERT_EQ(response[4], std::byte(0x07)); // Quantity of Outputs LSB
}

/********************************************************************************************************************

    Write Multiple Registers Tests \\
    The following tests are for the Write Multiple Registers function code (0x10). The tests cover the following scenarios: @n
        -  The response returns an exception when the address is invalid@n
        -  The response returns an exception when the quantity is invalid@n
        -  The response returns an exception when the byte count is invalid@n
        -  The response returns an exception when the range exceeds the maximum number of registers@n
        -  The response returns an exception when the data is not enough to write the registers@n
        -  The response returns the correct data when the registers are written@n

        @see WriteMultipleRegistersInvalidAddressResponse @n
        @see WriteMultipleRegistersInvalidQuantityResponse @n
        @see WriteMultipleRegistersRigthQuantityNotEnoughDataToWriteResponse @n
        @see WriteMultipleRegistersCorrectResponse @n

     ********************************************************************************************************************/

TEST_F(ModbusPDUTest, WriteMultipleRegistersInvalidAddressResponse) {

    // Modbus Area has 10 registers, so trying to write to register 11 should return an exception
    Modbus::PDU pdu({std::byte{0x10}, // Function Code: Write Multiple Registers (16)
                     std::byte{0x00}, // Starting Address MSB
                     std::byte{0x0B}, // Starting Address LSB; Starting Address = 11 (invalid)
                     std::byte{0x00}, // Quantity of Registers MSB
                     std::byte{0x02}, // Quantity of Registers LSB; Quantity of Registers = 2
                     std::byte{0x04}, // Byte Count = 2
                     std::byte{0xFF}, // Register Value MSB
                     std::byte{0xFF}, // Register Value LSB
                     std::byte{0xFF}, // Register Value MSB
                     std::byte{0xFF}}, // Register Value LSB
                    modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x90}); // Exception code: 0x80 + 0x10
    ASSERT_EQ(response[1],
              std::byte{0x02}); // Exception code: 0x02 (Illegal Data Address Exception) TODO: Double check this
}

TEST_F(ModbusPDUTest, WriteMultipleRegistersInvalidQuantityResponse) {

    // Modbus Area has 10 registers, so trying to write 11 registers should return an exception
    std::vector<std::byte> rawData = {std::byte{0x10}, // Function Code: Write Multiple Registers (16)
                                      std::byte{0x00},  // Starting Address MSB
                                      std::byte{0x01}, // Starting Address LSB; Starting Address = 1
                                      std::byte{0x00}, // Quantity of Registers MSB
                                      std::byte{
                                              0x0B} // Quantity of Registers LSB ; Quantity of Registers = 11 (invalid)
    };

    for (int i = 0; i < 12; i++) {
        rawData.push_back(std::byte{0xFF});
        rawData.push_back(std::byte{0xFF});
    }

    Modbus::PDU pdu(rawData, modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x90}); // Exception code: 0x80 + 0x10
    ASSERT_EQ(response[1], std::byte{0x03}); // Exception code: 0x03 (Illegal Data Value Exception)
}

TEST_F(ModbusPDUTest, WriteMultipleRegistersRigthQuantityNotEnoughDataToWriteResponse) {

    // Modbus Area has 10 registers, so trying to write 11 registers should return an exception
    std::vector<std::byte> rawData = {std::byte{0x10}, // Function Code: Write Multiple Registers (16)
                                      std::byte{0x00},  // Starting Address MSB
                                      std::byte{0x01}, // Starting Address LSB; Starting Address = 1
                                      std::byte{0x00}, // Quantity of Registers MSB
                                      std::byte{0x0B}, // Quantity of Registers LSB ; Quantity of Registers = 11
                                      std::byte{0x16}}; // Byte Count = 22
    for (int i = 0; i < 2; i++) {
        rawData.push_back(std::byte{0xFF});
        rawData.push_back(std::byte{0xFF});
    }

    Modbus::PDU pdu(rawData, modbusDataAreaWithTenRegistersEach);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 2);
    ASSERT_EQ(response[0], std::byte{0x90}); // Exception code: 0x80 + 0x10
    ASSERT_EQ(response[1], std::byte{0x03}); // Exception code: 0x03 (Illegal Data Value Exception)
}

TEST_F(ModbusPDUTest, WriteMultipleRegistersCorrectResponse) {

    // Write 123 registers starting from address 0
    std::vector<std::byte> rawData = {std::byte{0x10}, // Function Code: Write Multiple Registers (10)
                                      std::byte{0x00}, // Starting Address MSB
                                      std::byte{0x00}, // Starting Address LSB; Starting Address = 1
                                      std::byte{0x00}, // Quantity of Registers MSB
                                      std::byte{0x7B}, // Quantity of Registers LSB ; Quantity of Registers = 123
                                      std::byte{0xF6}}; // Byte Count = 123 * 2 = 246

    for (int i = 1; i < Modbus::MAX_HOLDING_REGISTERS + 1; i++) {
        rawData.push_back(std::byte{0xFF});
        rawData.push_back(std::byte{0xFF});
    }

    Modbus::PDU pdu(rawData, modbusDataAreaWithMaxRegisters);

    auto response = pdu.buildResponse();

    for (auto reg: modbusDataAreaWithMaxRegisters.getAllHoldingRegisters()) {
        ASSERT_EQ(reg.read(), 65535);
    }

    ASSERT_EQ(response.size(), 5);
    ASSERT_EQ(response[0], std::byte{0x10}); // Function Code: Write Multiple Registers (16)
    ASSERT_EQ(response[1], std::byte{0x00}); // Starting Address MSB
    ASSERT_EQ(response[2], std::byte{0x00}); // Starting Address LSB
    ASSERT_EQ(response[3], std::byte{0x00}); // Quantity of Registers MSB
    ASSERT_EQ(response[4], std::byte{0x7B}); // Quantity of Registers LSB
}

TEST(ModbusTest, BytesToMBAPReturnsCorrectMBAPForValidBytes) {
    std::vector<std::byte> bytes = {std::byte(0x01), // Transaction Identifier MSB
                                    std::byte(0x02),  // Transaction Identifier LSB
                                    std::byte(0x03),  // Protocol Identifier MSB
                                    std::byte(0x04), // Protocol Identifier LSB
                                    std::byte(0x05),  // Length MSB
                                    std::byte(0x06),  // Length LSB
                                    std::byte(0x07)}; // Unit Identifier
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
    std::vector<std::byte> bytes = {std::byte(0x01), // Transaction Identifier MSB
                                    std::byte(0x02), // Transaction Identifier LSB
                                    std::byte(0x03)}; // Protocol Identifier MSB

    EXPECT_THROW(Modbus::bytesToMBAP(bytes), std::invalid_argument);
}

TEST(ModbusTest, MBAPToBytesReturnsCorrectBytesForValidMBAP) {
    Modbus::MBAP mbap{};
    mbap.transactionIdentifier = 0x0102;
    mbap.protocolIdentifier = 0x0304;
    mbap.length = 0x0506;
    mbap.unitIdentifier = 0x01;
    std::vector<std::byte> expectedBytes = {std::byte(0x01), // Transaction Identifier MSB
                                            std::byte(0x02),  // Transaction Identifier LSB
                                            std::byte(0x03), // Protocol Identifier MSB
                                            std::byte(0x04),  // Protocol Identifier LSB
                                            std::byte(0x05),  // Length MSB
                                            std::byte(0x06),  // Length LSB
                                            std::byte(0x01)}; // Unit Identifier

    std::vector<std::byte> actualBytes = Modbus::MBAPToBytes(mbap);

    EXPECT_EQ(expectedBytes, actualBytes);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
