//
// Created by Luis Johnson on 3/16/24.
//
#include <gtest/gtest.h>
#include "Modbus.h"

TEST(ModbusFunctionCodeTest, ValidFunctionCodeReturnsTrue) {
    auto validCode = static_cast<std::byte>(ModbusFunctionCode::ReadCoils);
    ASSERT_TRUE(isValidModbusFunctionCode(validCode));
}

TEST(ModbusFunctionCodeTest, InvalidFunctionCodeReturnsFalse) {
    auto invalidCode = static_cast<std::byte>(0xFF);
    ASSERT_FALSE(isValidModbusFunctionCode(invalidCode));
}

TEST(ModbusFunctionCodeTest, LowerBoundaryFunctionCodeReturnsTrue) {
    auto lowerBoundaryCode = static_cast<std::byte>(ModbusFunctionCode::ReadCoils);
    ASSERT_TRUE(isValidModbusFunctionCode(lowerBoundaryCode));
}

TEST(ModbusFunctionCodeTest, UpperBoundaryFunctionCodeReturnsTrue) {
    auto upperBoundaryCode = static_cast<std::byte>(ModbusFunctionCode::ReadDeviceIdentification);
    ASSERT_TRUE(isValidModbusFunctionCode(upperBoundaryCode));
}


TEST(ModbusTest, FillWithZerosReturnsValueWithCorrectNumberOfDigits) {
    EXPECT_EQ(fillWithZeros(123, 5), "00123");
}

TEST(ModbusTest, FillWithZerosReturnsOriginalValueWhenLengthIsLessThanNumberOfDigits) {
    EXPECT_EQ(fillWithZeros(12345, 3), "12345");
}

TEST(ModbusTest, FillWithZerosReturnsZeroWhenValueIsZero) {
    EXPECT_EQ(fillWithZeros(0, 5), "00000");
}

TEST(ModbusTest, FillWithZerosReturnsValueWithZerosWhenLengthIsGreaterThanNumberOfDigits) {
    EXPECT_EQ(fillWithZeros(1, 5), "00001");
}

TEST(ModbusTest, FillWithZerosThrowsExceptionWhenValueIsNegative) {
    EXPECT_THROW(fillWithZeros(-1, 5), std::invalid_argument);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
