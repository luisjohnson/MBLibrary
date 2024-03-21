//
// Created by Luis Johnson on 3/17/24.
//
#include <gtest/gtest.h>
#include "ModbusDataArea.h"


TEST(ModbusDataAreaTest, InsertAndRetrieveCoil) {
    ModbusDataArea dataArea;
    ModbusCoil coil = ModbusCoil(1, true);
    dataArea.insertCoil(coil);
    auto allCoils = dataArea.getAllCoils();
    ASSERT_TRUE(allCoils.size() == 1);
    ASSERT_TRUE(allCoils[0].getAddress() == 1);
    ASSERT_TRUE(allCoils[0].read() == true);
}
//
//TEST(ModbusDataAreaTest, InsertAndRetrieveDiscreteInput) {
//    ModbusDataArea dataArea;
//    auto input = std::make_shared<ModbusDiscreteInput>(100, true);
//    dataArea.insertDiscreteInput(input);
//    auto retrievedInput = dataArea.getDiscreteInputs(100, 1).front();
//    ASSERT_EQ(retrievedInput, input);
//}
//
//TEST(ModbusDataAreaTest, InsertAndRetrieveHoldingRegister) {
//    ModbusDataArea dataArea;
//    auto holdingRegister = std::make_shared<ModbusHoldingRegister>(20, 1000);
//    dataArea.insertHoldingRegister(holdingRegister);
//    auto retrievedRegister = dataArea.getHoldingRegisters(20, 1).front();
//    ASSERT_EQ(retrievedRegister, holdingRegister);
//}
//
//TEST(ModbusDataAreaTest, InsertAndRetrieveInputRegister) {
//    ModbusDataArea dataArea;
//    auto inputRegister = std::make_shared<ModbusInputRegister>(10, 1000);
//    dataArea.insertInputRegister(inputRegister);
//    auto retrievedRegister = dataArea.getInputRegisters(10, 1).front();
//    ASSERT_EQ(retrievedRegister, inputRegister);
//}
//
//TEST(ModbusDataAreaTest, RetrieveNonExistentCoilThrowsException) {
//    ModbusDataArea dataArea;
//    ASSERT_THROW(dataArea.getCoils(0, 1), std::out_of_range);
//}
//
//TEST(ModbusDataAreaTest, RetrieveNonExistentDiscreteInputThrowsException) {
//    ModbusDataArea dataArea;
//    ASSERT_THROW(dataArea.getDiscreteInputs(0, 1), std::out_of_range);
//}
//
//TEST(ModbusDataAreaTest, RetrieveNonExistentHoldingRegisterThrowsException) {
//    ModbusDataArea dataArea;
//    ASSERT_THROW(dataArea.getHoldingRegisters(0, 1), std::out_of_range);
//}
//
//TEST(ModbusDataAreaTest, RetrieveNonExistentInputRegisterThrowsException) {
//    ModbusDataArea dataArea;
//    ASSERT_THROW(dataArea.getInputRegisters(0, 1), std::out_of_range);
//}
//
//TEST(ModbusDataAreaTest, InsertAndRetrieveMultipleCoils) {
//    ModbusDataArea dataArea;
//    auto coil1 = std::make_shared<ModbusCoil>(1, true);
//    auto coil2 = std::make_shared<ModbusCoil>(2, false);
//    dataArea.insertCoil(coil1);
//    dataArea.insertCoil(coil2);
//    auto retrievedCoils = dataArea.getCoils(1, 2);
//    ASSERT_EQ(retrievedCoils[0], coil1);
//    ASSERT_EQ(retrievedCoils[1], coil2);
//}
//
//TEST(ModbusDataAreaTest, InsertAndRetrieveMultipleDiscreteInputs) {
//    ModbusDataArea dataArea;
//    auto input1 = std::make_shared<ModbusDiscreteInput>(100, true);
//    auto input2 = std::make_shared<ModbusDiscreteInput>(101, false);
//    dataArea.insertDiscreteInput(input1);
//    dataArea.insertDiscreteInput(input2);
//    auto retrievedInputs = dataArea.getDiscreteInputs(100, 2);
//    ASSERT_EQ(retrievedInputs[0], input1);
//    ASSERT_EQ(retrievedInputs[1], input2);
//}
//
//TEST(ModbusDataAreaTest, InsertAndRetrieveMultipleHoldingRegisters) {
//    ModbusDataArea dataArea;
//    auto holdingRegister1 = std::make_shared<ModbusHoldingRegister>(20, 1000);
//    auto holdingRegister2 = std::make_shared<ModbusHoldingRegister>(21, 2000);
//    dataArea.insertHoldingRegister(holdingRegister1);
//    dataArea.insertHoldingRegister(holdingRegister2);
//    auto retrievedRegisters = dataArea.getHoldingRegisters(20, 2);
//    ASSERT_EQ(retrievedRegisters[0], holdingRegister1);
//    ASSERT_EQ(retrievedRegisters[1], holdingRegister2);
//}
//
//TEST(ModbusDataAreaTest, InsertAndRetrieveMultipleInputRegisters) {
//    ModbusDataArea dataArea;
//    auto inputRegister1 = std::make_shared<ModbusInputRegister>(10, 1000);
//    auto inputRegister2 = std::make_shared<ModbusInputRegister>(11, 2000);
//    dataArea.insertInputRegister(inputRegister1);
//    dataArea.insertInputRegister(inputRegister2);
//    auto retrievedRegisters = dataArea.getInputRegisters(10, 2);
//    ASSERT_EQ(retrievedRegisters[0], inputRegister1);
//    ASSERT_EQ(retrievedRegisters[1], inputRegister2);
//}
//
//TEST(ModbusDataAreaTest, SortCoils) {
//    ModbusDataArea dataArea;
//    auto coil1 = std::make_shared<ModbusCoil>(2, true);
//    auto coil2 = std::make_shared<ModbusCoil>(1, false);
//    dataArea.insertCoil(coil1);
//    dataArea.insertCoil(coil2);
//    auto retrievedCoils = dataArea.getAllCoils();
//    ASSERT_EQ(retrievedCoils[0]->getAddress(), 1);
//    ASSERT_EQ(retrievedCoils[1]->getAddress(), 2);
//}
//
//TEST(ModbusDataAreaTest, SortDiscreteInputs) {
//    ModbusDataArea dataArea;
//    auto input1 = std::make_shared<ModbusDiscreteInput>(101, true);
//    auto input2 = std::make_shared<ModbusDiscreteInput>(100, false);
//    dataArea.insertDiscreteInput(input1);
//    dataArea.insertDiscreteInput(input2);
//    auto retrievedInputs = dataArea.getAllDiscreteInputs();
//    ASSERT_EQ(retrievedInputs[0]->getAddress(), 100);
//    ASSERT_EQ(retrievedInputs[1]->getAddress(), 101);
//}
//
//TEST(ModbusDataAreaTest, SortHoldingRegisters) {
//    ModbusDataArea dataArea;
//    auto holdingRegister1 = std::make_shared<ModbusHoldingRegister>(21, 1000);
//    auto holdingRegister2 = std::make_shared<ModbusHoldingRegister>(20, 2000);
//    dataArea.insertHoldingRegister(holdingRegister1);
//    dataArea.insertHoldingRegister(holdingRegister2);
//    auto retrievedRegisters = dataArea.getAllHoldingRegisters();
//    ASSERT_EQ(retrievedRegisters[0]->getAddress(), 20);
//    ASSERT_EQ(retrievedRegisters[1]->getAddress(), 21);
//}
//
//TEST(ModbusDataAreaTest, SortInputRegisters) {
//    ModbusDataArea dataArea;
//    auto inputRegister1 = std::make_shared<ModbusInputRegister>(11, 1000);
//    auto inputRegister2 = std::make_shared<ModbusInputRegister>(10, 2000);
//    dataArea.insertInputRegister(inputRegister1);
//    dataArea.insertInputRegister(inputRegister2);
//    auto retrievedRegisters = dataArea.getAllInputRegisters();
//    ASSERT_EQ(retrievedRegisters[0]->getAddress(), 10);
//    ASSERT_EQ(retrievedRegisters[1]->getAddress(), 11);
//}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}