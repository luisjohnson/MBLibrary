//
// Created by Luis Johnson on 3/16/24.
//
#include <gtest/gtest.h>
#include <memory>
#include <cstddef>
#include "Modbus.h"


TEST(ModbusTest, FillWithZerosReturnsValueWithCorrectNumberOfDigits) {
    EXPECT_EQ(Modbus::fillWithZeros(123, 5), "00123");
}

TEST(ModbusTest, FillWithZerosReturnsOriginalValueWhenLengthIsLessThanNumberOfDigits) {
    EXPECT_EQ(Modbus::fillWithZeros(12345, 3), "12345");
}

TEST(ModbusTest, FillWithZerosReturnsZeroWhenValueIsZero) {
    EXPECT_EQ(Modbus::fillWithZeros(0, 5), "00000");
}

TEST(ModbusTest, FillWithZerosReturnsValueWithZerosWhenLengthIsGreaterThanNumberOfDigits) {
    EXPECT_EQ(Modbus::fillWithZeros(1, 5), "00001");
}

TEST(ModbusTest, FillWithZerosThrowsExceptionWhenValueIsNegative) {
    EXPECT_THROW(Modbus::fillWithZeros(-1, 5), std::invalid_argument);
}

TEST(ModbusCoilTest, GetAddressWithHexPrefixReturnsCorrectFormat) {
    Modbus::Coil coil(123, true);
    EXPECT_EQ("0x00123", coil.getAddressWithHexPrefix());
}

TEST(ModbusDiscreteInputTest, GetAddressWithHexPrefixReturnsCorrectFormat) {
    Modbus::DiscreteInput input(123, true);
    ASSERT_EQ("1x00123", input.getAddressWithHexPrefix());
}

TEST(ModbusInputRegisterTest, GetAddressWithHexPrefixReturnsCorrectFormat) {
    Modbus::InputRegister reg(123, 0xFFFF);
    ASSERT_EQ("3x00123", reg.getAddressWithHexPrefix());
}

TEST(ModbusHoldingRegisterTest, GetAddressWithHexPrefixReturnsCorrectFormat) {
    Modbus::HoldingRegister reg(123, 0xFFFF);
    ASSERT_EQ("4x00123", reg.getAddressWithHexPrefix());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
