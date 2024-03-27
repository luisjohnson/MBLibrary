//
// Created by Luis Johnson on 3/17/24.
//
#include <gtest/gtest.h>
#include "ModbusDataArea.h"

class ModbusDataAreaTestWithFixture : public ::testing::Test {
protected:
    std::shared_ptr<ModbusDataArea> modbusDataArea;

    void SetUp() override {
        modbusDataArea = std::make_shared<ModbusDataArea>();
    }
};


TEST(ModbusDataAreaTest, InsertAndRetrieveCoil) {
    ModbusDataArea dataArea;
    ModbusCoil coil = ModbusCoil(1, true);
    dataArea.insertCoil(coil);
    auto allCoils = dataArea.getAllCoils();
    ASSERT_TRUE(allCoils.size() == 1);
    ASSERT_TRUE(allCoils[0].getAddress() == 1);
    ASSERT_TRUE(allCoils[0].read() == true);
}

TEST(ModbusDataAreaTest, InsertAndRetrieveDiscreteInput) {
    ModbusDataArea dataArea;
    auto input = ModbusDiscreteInput(100, true);
    dataArea.insertDiscreteInput(input);
    auto retrievedInput = dataArea.getDiscreteInputs(100, 1);
    ASSERT_TRUE(retrievedInput.size() == 1);
    ASSERT_TRUE(retrievedInput[0].getAddress() == 100);
    ASSERT_TRUE(retrievedInput[0].read() == true);
}

TEST(ModbusDataAreaTest, InsertAndRetrieveHoldingRegister) {
    ModbusDataArea dataArea;
    auto holdingRegister = ModbusHoldingRegister(20, 1000);
    dataArea.insertHoldingRegister(holdingRegister);
    auto retrievedRegister = dataArea.getHoldingRegisters(20, 1).front();
    ASSERT_TRUE(retrievedRegister.getAddress() == 20);
    ASSERT_TRUE(retrievedRegister.read() == 1000);

}

TEST(ModbusDataAreaTest, InsertAndRetrieveInputRegister) {
    ModbusDataArea dataArea;
    auto inputRegister = ModbusInputRegister(10, 1000);
    dataArea.insertInputRegister(inputRegister);
    auto retrievedRegister = dataArea.getInputRegisters(10, 1).front();
    ASSERT_TRUE(retrievedRegister.getAddress() == 10);
    ASSERT_TRUE(retrievedRegister.read() == 1000);
}

TEST(ModbusDataAreaTest, RetrieveNonExistentCoilThrowsException) {
    ModbusDataArea dataArea;
    ASSERT_THROW(dataArea.getCoils(0, 1), std::out_of_range);
}

TEST(ModbusDataAreaTest, RetrieveNonExistentDiscreteInputThrowsException) {
    ModbusDataArea dataArea;
    ASSERT_THROW(dataArea.getDiscreteInputs(0, 1), std::out_of_range);
}

TEST(ModbusDataAreaTest, RetrieveNonExistentHoldingRegisterThrowsException) {
    ModbusDataArea dataArea;
    ASSERT_THROW(dataArea.getHoldingRegisters(0, 1), std::out_of_range);
}

TEST(ModbusDataAreaTest, RetrieveNonExistentInputRegisterThrowsException) {
    ModbusDataArea dataArea;
    ASSERT_THROW(dataArea.getInputRegisters(0, 1), std::out_of_range);
}

TEST(ModbusDataAreaTest, InsertAndRetrieveMultipleCoils) {
    ModbusDataArea dataArea;
    auto coil1 = ModbusCoil(1, true);
    auto coil2 = ModbusCoil(2, false);
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
    ModbusDataArea dataArea;
    auto input1 = ModbusDiscreteInput(100, true);
    auto input2 = ModbusDiscreteInput(101, false);
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
    ModbusDataArea dataArea;
    auto holdingRegister1 = ModbusHoldingRegister(20, 1000);
    auto holdingRegister2 = ModbusHoldingRegister(21, 2000);
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
    ModbusDataArea dataArea;
    auto inputRegister1 = ModbusInputRegister(10, 1000);
    auto inputRegister2 = ModbusInputRegister(11, 2000);
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
    ModbusDataArea dataArea;
    auto coil1 = ModbusCoil(2, true);
    auto coil2 = ModbusCoil(1, false);
    dataArea.insertCoil(coil1);
    dataArea.insertCoil(coil2);
    auto retrievedCoils = dataArea.getAllCoils();
    ASSERT_EQ(retrievedCoils[0].getAddress(), 1);
    ASSERT_EQ(retrievedCoils[1].getAddress(), 2);
}

TEST(ModbusDataAreaTest, SortDiscreteInputs) {
    ModbusDataArea dataArea;
    auto input1 = ModbusDiscreteInput(101, true);
    auto input2 = ModbusDiscreteInput(100, false);
    dataArea.insertDiscreteInput(input1);
    dataArea.insertDiscreteInput(input2);
    auto retrievedInputs = dataArea.getAllDiscreteInputs();
    ASSERT_EQ(retrievedInputs[0].getAddress(), 100);
    ASSERT_EQ(retrievedInputs[1].getAddress(), 101);
}

TEST(ModbusDataAreaTest, SortHoldingRegisters) {
    ModbusDataArea dataArea;
    auto holdingRegister1 = ModbusHoldingRegister(21, 1000);
    auto holdingRegister2 = ModbusHoldingRegister(20, 2000);
    dataArea.insertHoldingRegister(holdingRegister1);
    dataArea.insertHoldingRegister(holdingRegister2);
    auto retrievedRegisters = dataArea.getAllHoldingRegisters();
    ASSERT_EQ(retrievedRegisters[0].getAddress(), 20);
    ASSERT_EQ(retrievedRegisters[1].getAddress(), 21);
}

TEST(ModbusDataAreaTest, SortInputRegisters) {
    ModbusDataArea dataArea;
    auto inputRegister1 = ModbusInputRegister(11, 1000);
    auto inputRegister2 = ModbusInputRegister(10, 2000);
    dataArea.insertInputRegister(inputRegister1);
    dataArea.insertInputRegister(inputRegister2);
    auto retrievedRegisters = dataArea.getAllInputRegisters();
    ASSERT_EQ(retrievedRegisters[0].getAddress(), 10);
    ASSERT_EQ(retrievedRegisters[1].getAddress(), 11);
}

TEST(ModbusDataAreaTest, InsertCoilThrowsExceptionWhenMaxCoilsExceeded) {
    ModbusDataArea dataArea;
    for (int i = 1; i < MAX_COILS + 1; i++) {
        ASSERT_NO_THROW(dataArea.insertCoil(ModbusCoil(i, true)));
    }
    ASSERT_THROW(dataArea.insertCoil(ModbusCoil(2001, true)), std::range_error);
}

TEST(ModbusDataAreaTest, InsertInputRegisterThorwsExceptionWhenMaxInputRegistersExceeded) {
    ModbusDataArea dataArea;
    for (int i = 1; i < MAX_INPUT_REGISTERS + 1; i++) {
        ASSERT_NO_THROW(dataArea.insertInputRegister(ModbusInputRegister(i, 1000)));
    }
    ASSERT_THROW(dataArea.insertInputRegister(ModbusInputRegister(2001, 1000)), std::range_error);
}

// TODO Fix fixture tests. They are getting frozen.
TEST_F(ModbusDataAreaTestWithFixture, generateCoilsWithZeros) {
    modbusDataArea->generateCoils(0, 10, ValueGenerationType::Zeros);
    auto coils = modbusDataArea->getCoils(0, 10);
    for (ModbusCoil coil : coils) {
        EXPECT_EQ(coil.read(), false);
    }
}

TEST_F(ModbusDataAreaTestWithFixture, generateDiscreteInputsWithOnes) {
    modbusDataArea->generateDiscreteInputs(0, 10, ValueGenerationType::Ones);
    auto inputs = modbusDataArea->getDiscreteInputs(0, 10);
    for (ModbusDiscreteInput input : inputs) {
        EXPECT_EQ(input.read(), true);
    }
}

TEST_F(ModbusDataAreaTestWithFixture, generateHoldingRegistersWithRandomValues) {
    modbusDataArea->generateHoldingRegisters(0, 10, ValueGenerationType::Random);
    auto registers = modbusDataArea->getHoldingRegisters(0, 10);
    int previousValue = registers[0].read();
    for (int i = 1; i < registers.size(); i++) {
        EXPECT_NE(registers[i].read(), previousValue);
        previousValue = registers[i].read();
    }
}

TEST_F(ModbusDataAreaTestWithFixture, generateInputRegistersWithIncrementalValues) {
    modbusDataArea->generateInputRegisters(0, 10, ValueGenerationType::Incremental);
    auto registers = modbusDataArea->getInputRegisters(0, 10);
    for (int i = 0; i < registers.size(); i++) {
        EXPECT_EQ(registers[i].read(), i);
    }
}

TEST_F(ModbusDataAreaTestWithFixture, generateCoilsWithInvalidCount) {
    EXPECT_THROW(modbusDataArea->generateCoils(0, 2001, ValueGenerationType::Zeros), std::out_of_range);
}

TEST_F(ModbusDataAreaTestWithFixture, generateDiscreteInputsWithInvalidCount) {
    EXPECT_THROW(modbusDataArea->generateDiscreteInputs(0, 2001, ValueGenerationType::Zeros), std::out_of_range);
}

TEST_F(ModbusDataAreaTestWithFixture, generateHoldingRegistersWithInvalidCount) {
    EXPECT_THROW(modbusDataArea->generateHoldingRegisters(0, 126, ValueGenerationType::Zeros), std::out_of_range);
}

TEST_F(ModbusDataAreaTestWithFixture, generateInputRegistersWithInvalidCount) {
    EXPECT_THROW(modbusDataArea->generateInputRegisters(0, 126, ValueGenerationType::Zeros), std::out_of_range);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}