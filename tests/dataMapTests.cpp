//
// Created by Luis Johnson on 3/17/24.
//
#include <gtest/gtest.h>
#include "ModbusDataArea.h"
#include "Modbus.h"

class ModbusDataAreaTestWithFixture : public ::testing::Test {
protected:
    std::shared_ptr<Modbus::DataArea> modbusDataArea;

    void SetUp() override {
        modbusDataArea = std::make_shared<Modbus::DataArea>();
    }
};


TEST(ModbusDataAreaTest, InsertAndRetrieveCoil) {
    Modbus::DataArea dataArea;
    Modbus::Coil coil = Modbus::Coil(1, true);
    dataArea.insertCoil(coil);
    auto allCoils = dataArea.getAllCoils();
    ASSERT_TRUE(allCoils.size() == 1);
    ASSERT_TRUE(allCoils[0].getAddress() == 1);
    ASSERT_TRUE(allCoils[0].read() == true);
}

TEST(ModbusDataAreaTest, InsertAndRetrieveDiscreteInput) {
    Modbus::DataArea dataArea;
    auto input = Modbus::DiscreteInput(100, true);
    dataArea.insertDiscreteInput(input);
    auto retrievedInput = dataArea.getDiscreteInputs(100, 1);
    ASSERT_TRUE(retrievedInput.size() == 1);
    ASSERT_TRUE(retrievedInput[0].getAddress() == 100);
    ASSERT_TRUE(retrievedInput[0].read() == true);
}

TEST(ModbusDataAreaTest, InsertAndRetrieveHoldingRegister) {
    Modbus::DataArea dataArea;
    auto holdingRegister = Modbus::HoldingRegister(20, 1000);
    dataArea.insertHoldingRegister(holdingRegister);
    auto retrievedRegister = dataArea.getHoldingRegisters(20, 1).front();
    ASSERT_TRUE(retrievedRegister.getAddress() == 20);
    ASSERT_TRUE(retrievedRegister.read() == 1000);

}

TEST(ModbusDataAreaTest, InsertAndRetrieveInputRegister) {
    Modbus::DataArea dataArea;
    auto inputRegister = Modbus::InputRegister(10, 1000);
    dataArea.insertInputRegister(inputRegister);
    auto retrievedRegister = dataArea.getInputRegisters(10, 1).front();
    ASSERT_TRUE(retrievedRegister.getAddress() == 10);
    ASSERT_TRUE(retrievedRegister.read() == 1000);
}

TEST(ModbusDataAreaTest, RetrieveNonExistentCoilThrowsException) {
    Modbus::DataArea dataArea;
    ASSERT_THROW(dataArea.getCoils(0, 1), std::out_of_range);
}

TEST(ModbusDataAreaTest, RetrieveNonExistentDiscreteInputThrowsException) {
    Modbus::DataArea dataArea;
    ASSERT_THROW(dataArea.getDiscreteInputs(0, 1), std::out_of_range);
}

TEST(ModbusDataAreaTest, RetrieveNonExistentHoldingRegisterThrowsException) {
    Modbus::DataArea dataArea;
    ASSERT_THROW(dataArea.getHoldingRegisters(0, 1), std::out_of_range);
}

TEST(ModbusDataAreaTest, RetrieveNonExistentInputRegisterThrowsException) {
    Modbus::DataArea dataArea;
    ASSERT_THROW(dataArea.getInputRegisters(0, 1), std::out_of_range);
}

TEST(ModbusDataAreaTest, InsertAndRetrieveMultipleCoils) {
    Modbus::DataArea dataArea;
    auto coil1 = Modbus::Coil(1, true);
    auto coil2 = Modbus::Coil(2, false);
    dataArea.insertCoil(coil1);
    dataArea.insertCoil(coil2);
    auto retrievedCoils = dataArea.getCoils(1, 2);
    ASSERT_EQ(retrievedCoils.size(), 2);
    ASSERT_EQ(retrievedCoils[0].getAddress(), 1);
    ASSERT_EQ(retrievedCoils[1].getAddress(), 2);
    ASSERT_EQ(retrievedCoils[0].read(), true);
    ASSERT_EQ(retrievedCoils[1].read(), false);
}

TEST(ModbusDataAreaTest, InsertAndRetrieveMultipleDiscreteInputs) {
    Modbus::DataArea dataArea;
    auto input1 = Modbus::DiscreteInput(100, true);
    auto input2 = Modbus::DiscreteInput(101, false);
    dataArea.insertDiscreteInput(input1);
    dataArea.insertDiscreteInput(input2);
    auto retrievedInputs = dataArea.getDiscreteInputs(100, 2);
    ASSERT_EQ(retrievedInputs.size(), 2);
    ASSERT_EQ(retrievedInputs[0].getAddress(), 100);
    ASSERT_EQ(retrievedInputs[1].getAddress(), 101);
    ASSERT_EQ(retrievedInputs[0].read(), true);
    ASSERT_EQ(retrievedInputs[1].read(), false);
}

TEST(ModbusDataAreaTest, InsertAndRetrieveMultipleHoldingRegisters) {
    Modbus::DataArea dataArea;
    auto holdingRegister1 = Modbus::HoldingRegister(20, 1000);
    auto holdingRegister2 = Modbus::HoldingRegister(21, 2000);
    dataArea.insertHoldingRegister(holdingRegister1);
    dataArea.insertHoldingRegister(holdingRegister2);
    auto retrievedRegisters = dataArea.getHoldingRegisters(20, 2);
    ASSERT_TRUE(retrievedRegisters.size() == 2);
    ASSERT_EQ(retrievedRegisters[0].getAddress(), 20);
    ASSERT_EQ(retrievedRegisters[1].getAddress(), 21);
    ASSERT_EQ(retrievedRegisters[0].read(), 1000);
    ASSERT_EQ(retrievedRegisters[1].read(), 2000);

}

TEST(ModbusDataAreaTest, InsertAndRetrieveMultipleInputRegisters) {
    Modbus::DataArea dataArea;
    auto inputRegister1 = Modbus::InputRegister(10, 1000);
    auto inputRegister2 = Modbus::InputRegister(11, 2000);
    dataArea.insertInputRegister(inputRegister1);
    dataArea.insertInputRegister(inputRegister2);
    auto retrievedRegisters = dataArea.getInputRegisters(10, 2);
    ASSERT_TRUE(retrievedRegisters.size() == 2);
    ASSERT_EQ(retrievedRegisters[0].getAddress(), 10);
    ASSERT_EQ(retrievedRegisters[1].getAddress(), 11);
    ASSERT_EQ(retrievedRegisters[0].read(), 1000);
    ASSERT_EQ(retrievedRegisters[1].read(), 2000);
}

TEST(ModbusDataAreaTest, SortCoils) {
    Modbus::DataArea dataArea;
    auto coil1 = Modbus::Coil(2, true);
    auto coil2 = Modbus::Coil(1, false);
    dataArea.insertCoil(coil1);
    dataArea.insertCoil(coil2);
    auto retrievedCoils = dataArea.getAllCoils();
    ASSERT_EQ(retrievedCoils[0].getAddress(), 1);
    ASSERT_EQ(retrievedCoils[1].getAddress(), 2);
}

TEST(ModbusDataAreaTest, SortDiscreteInputs) {
    Modbus::DataArea dataArea;
    auto input1 = Modbus::DiscreteInput(101, true);
    auto input2 = Modbus::DiscreteInput(100, false);
    dataArea.insertDiscreteInput(input1);
    dataArea.insertDiscreteInput(input2);
    auto retrievedInputs = dataArea.getAllDiscreteInputs();
    ASSERT_EQ(retrievedInputs[0].getAddress(), 100);
    ASSERT_EQ(retrievedInputs[1].getAddress(), 101);
}

TEST(ModbusDataAreaTest, SortHoldingRegisters) {
    Modbus::DataArea dataArea;
    auto holdingRegister1 = Modbus::HoldingRegister(21, 1000);
    auto holdingRegister2 = Modbus::HoldingRegister(20, 2000);
    dataArea.insertHoldingRegister(holdingRegister1);
    dataArea.insertHoldingRegister(holdingRegister2);
    auto retrievedRegisters = dataArea.getAllHoldingRegisters();
    ASSERT_EQ(retrievedRegisters[0].getAddress(), 20);
    ASSERT_EQ(retrievedRegisters[1].getAddress(), 21);
}

TEST(ModbusDataAreaTest, SortInputRegisters) {
    Modbus::DataArea dataArea;
    auto inputRegister1 = Modbus::InputRegister(11, 1000);
    auto inputRegister2 = Modbus::InputRegister(10, 2000);
    dataArea.insertInputRegister(inputRegister1);
    dataArea.insertInputRegister(inputRegister2);
    auto retrievedRegisters = dataArea.getAllInputRegisters();
    ASSERT_EQ(retrievedRegisters[0].getAddress(), 10);
    ASSERT_EQ(retrievedRegisters[1].getAddress(), 11);
}

TEST(ModbusDataAreaTest, InsertCoilThrowsExceptionWhenMaxCoilsExceeded) {
    Modbus::DataArea dataArea;
    for (int i = 1; i < Modbus::MAX_COILS + 1; i++) {
        ASSERT_NO_THROW(dataArea.insertCoil(Modbus::Coil(i, true)));
    }
    ASSERT_THROW(dataArea.insertCoil(Modbus::Coil(2001, true)), std::range_error);
}

TEST(ModbusDataAreaTest, InsertInputRegisterThorwsExceptionWhenMaxInputRegistersExceeded) {
    Modbus::DataArea dataArea;
    for (int i = 1; i < Modbus::MAX_INPUT_REGISTERS + 1; i++) {
        ASSERT_NO_THROW(dataArea.insertInputRegister(Modbus::InputRegister(i, 1000)));
    }
    ASSERT_THROW(dataArea.insertInputRegister(Modbus::InputRegister(2001, 1000)), std::range_error);
}

TEST(ModbusDataAreaTest, RegisterExists) {
    Modbus::DataArea dataArea;
    for (int i = 0; i < 10 + 1; i++) {
        ASSERT_NO_THROW(dataArea.insertHoldingRegister(Modbus::HoldingRegister(i, 1000)));
    }
    ASSERT_THROW(dataArea.insertHoldingRegister(Modbus::HoldingRegister(1, 1000)), std::invalid_argument);
}

TEST_F(ModbusDataAreaTestWithFixture, generateCoilsWithZeros) {
    modbusDataArea->generateCoils(0, 10, Modbus::ValueGenerationType::Zeros);
    auto coils = modbusDataArea->getCoils(0, 10);
    for (Modbus::Coil coil : coils) {
        EXPECT_EQ(coil.read(), false);
    }
}

TEST_F(ModbusDataAreaTestWithFixture, generateDiscreteInputsWithOnes) {
    modbusDataArea->generateDiscreteInputs(0, 10, Modbus::ValueGenerationType::Ones);
    auto inputs = modbusDataArea->getDiscreteInputs(0, 10);
    for (Modbus::DiscreteInput input : inputs) {
        EXPECT_EQ(input.read(), true);
    }
}

TEST_F(ModbusDataAreaTestWithFixture, generateHoldingRegistersWithRandomValues) {
    modbusDataArea->generateHoldingRegisters(0, 10, Modbus::ValueGenerationType::Random);
    auto registers = modbusDataArea->getHoldingRegisters(0, 10);
    int previousValue = registers[0].read();
    for (int i = 1; i < registers.size(); i++) {
        EXPECT_NE(registers[i].read(), previousValue);
        previousValue = registers[i].read();
    }
}

TEST_F(ModbusDataAreaTestWithFixture, generateInputRegistersWithIncrementalValues) {
    modbusDataArea->generateInputRegisters(0, 10, Modbus::ValueGenerationType::Incremental);
    auto registers = modbusDataArea->getInputRegisters(0, 10);
    for (int i = 0; i < registers.size(); i++) {
        EXPECT_EQ(registers[i].read(), i);
    }
}

TEST_F(ModbusDataAreaTestWithFixture, generateCoilsWithInvalidCount) {
    EXPECT_THROW(modbusDataArea->generateCoils(0, 2001, Modbus::ValueGenerationType::Zeros), std::out_of_range);
}

TEST_F(ModbusDataAreaTestWithFixture, generateDiscreteInputsWithInvalidCount) {
    EXPECT_THROW(modbusDataArea->generateDiscreteInputs(0, 2001, Modbus::ValueGenerationType::Zeros), std::out_of_range);
}

TEST_F(ModbusDataAreaTestWithFixture, generateHoldingRegistersWithInvalidCount) {
    EXPECT_THROW(modbusDataArea->generateHoldingRegisters(0, 126, Modbus::ValueGenerationType::Zeros), std::out_of_range);
}

TEST_F(ModbusDataAreaTestWithFixture, generateInputRegistersWithInvalidCount) {
    EXPECT_THROW(modbusDataArea->generateInputRegisters(0, 126, Modbus::ValueGenerationType::Zeros), std::out_of_range);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}