#include <gtest/gtest.h>
#include <cstddef>
#include <ModbusUtilities.h>
#include <Modbus.h>


class UtilityTest : public ::testing::Test {

protected:
    std::vector<Modbus::Coil> coils;
    std::vector<Modbus::DiscreteInput> discreteInputs;
    std::byte msb{};
    std::byte lsb{};
    uint16_t expected{};

    std::vector<Modbus::HoldingRegister> holdingRegisters;
    std::vector<Modbus::InputRegister> inputRegisters;

    std::vector<Modbus::InputRegister> emptyInputRegisters{};
    std::vector<Modbus::HoldingRegister> emptyHoldingRegisters{};

    void SetUp() override {
        // Initialize the coils and discrete inputs
        for (int i = 0; i < 10; ++i) {
            coils.emplace_back(i, i % 2 == 0);
            discreteInputs.emplace_back(i, i % 2 == 0);
        }

        holdingRegisters = {Modbus::HoldingRegister(1, 0x1234), Modbus::HoldingRegister(2, 0x5678)};
        inputRegisters = {Modbus::InputRegister(1, 0x9ABC), Modbus::InputRegister(2, 0xDEF0)};
    }
};

TEST_F(UtilityTest, packBooleanRegistersIntoBytes_Coils) {
    auto packedCoils = Modbus::Utilities::packBooleanRegistersIntoBytes(coils);

    // Check the size of the packed coils
    EXPECT_EQ(packedCoils.size(), 2);

    // Check the values of the packed coils
    EXPECT_EQ(packedCoils[0], std::byte{0b01010101});
    EXPECT_EQ(packedCoils[1], std::byte{0b00000001});
}

TEST_F(UtilityTest, packBooleanRegistersIntoBytes_DiscreteInputs) {
    auto packedDiscreteInputs = Modbus::Utilities::packBooleanRegistersIntoBytes(discreteInputs);

    // Check the size of the packed discrete inputs
    EXPECT_EQ(packedDiscreteInputs.size(), 2);

    // Check the values of the packed discrete inputs
    EXPECT_EQ(packedDiscreteInputs[0], std::byte{0b01010101});
    EXPECT_EQ(packedDiscreteInputs[1], std::byte{0b00000001});
}

TEST_F(UtilityTest, packBooleanRegistersIntoBytes_EmptyVector) {
    std::vector<Modbus::Coil> emptyCoils;
    auto packedCoils = Modbus::Utilities::packBooleanRegistersIntoBytes(emptyCoils);

    // Check the size of the packed coils
    EXPECT_TRUE(packedCoils.empty());
}

// TEST(UtilityTest, packBooleanRegistersIntoBytes_WrongType) {
//     std::vector<std::shared_ptr<InputRegister>> inputRegisters;
//     inputRegisters.push_back(std::make_shared<InputRegister>(1, 1000));
//
//     // Check that the function throws a static_assert
//     EXPECT_ANY_THROW(packBooleanRegistersIntoBytes(inputRegisters));
// }

TEST_F(UtilityTest, PackHoldingRegistersIntoBytesReturnsCorrectBytes) {
    auto bytes = Modbus::Utilities::packIntegerRegistersIntoBytes(holdingRegisters);
    // Check the bytes
    EXPECT_EQ(bytes.size(), 4);
    EXPECT_EQ(bytes[0], static_cast<std::byte>(0x12));
    EXPECT_EQ(bytes[1], static_cast<std::byte>(0x34));
    EXPECT_EQ(bytes[2], static_cast<std::byte>(0x56));
    EXPECT_EQ(bytes[3], static_cast<std::byte>(0x78));

}

TEST_F(UtilityTest, PackInputRegistersIntoBytesReturnsCorrectBytes) {
    auto bytes = Modbus::Utilities::packIntegerRegistersIntoBytes(inputRegisters);
    // Check the bytes
    EXPECT_EQ(bytes.size(), 4);
    EXPECT_EQ(bytes[0], static_cast<std::byte>(0x9A));
    EXPECT_EQ(bytes[1], static_cast<std::byte>(0xBC));
    EXPECT_EQ(bytes[2], static_cast<std::byte>(0xDE));
    EXPECT_EQ(bytes[3], static_cast<std::byte>(0xF0));
}


TEST_F(UtilityTest, PackEmptyHoldingRegistersIntoBytesReturnsEmptyBytes) {

    auto bytes = Modbus::Utilities::packIntegerRegistersIntoBytes(emptyHoldingRegisters);
    // Check the bytes
    EXPECT_TRUE(bytes.empty());
}

TEST_F(UtilityTest, PackEmptyInputRegistersIntoBytesReturnsEmptyBytes) {
    auto bytes = Modbus::Utilities::packIntegerRegistersIntoBytes(emptyInputRegisters);
    // Check the bytes
    EXPECT_TRUE(bytes.empty());
}

TEST_F(UtilityTest, TestNormalValues) {
    msb = std::byte(0xAB);
    lsb = std::byte(0xCD);
    expected = 0xABCD;

    ASSERT_EQ(Modbus::Utilities::twoBytesToUint16(msb, lsb), expected);
}

TEST_F(UtilityTest, TestMaximumValues) {
    msb = std::byte(0xFF);
    lsb = std::byte(0xFF);
    expected = 0xFFFF;

    ASSERT_EQ(Modbus::Utilities::twoBytesToUint16(msb, lsb), expected);
}

TEST_F(UtilityTest, TestMinimumValues) {
    msb = std::byte(0x00);
    lsb = std::byte(0x00);
    expected = 0x0000;

    ASSERT_EQ(Modbus::Utilities::twoBytesToUint16(msb, lsb), expected);
}

TEST_F(UtilityTest, TestRandomValues) {
    msb = std::byte(0x1F);
    lsb = std::byte(0x3E);
    expected = 0x1F3E;

    ASSERT_EQ(Modbus::Utilities::twoBytesToUint16(msb, lsb), expected);
}

TEST_F(UtilityTest, GenerateRandomBooleanReturnsTrueOrFalse) {
    bool result = Modbus::Utilities::generateRandomBoolean();
    ASSERT_TRUE(result == true || result == false);
}

TEST_F(UtilityTest, GenerateRandomIntegerReturnsValueWithinRange) {
    int min = 0;
    int max = 100;
    int result = Modbus::Utilities::generateRandomInteger(min, max);
    ASSERT_GE(result, min);
    ASSERT_LE(result, max);
}

TEST_F(UtilityTest, GenerateRandomIntegerReturnsValueWithinNegativeRange) {
    int min = -100;
    int max = -1;
    int result = Modbus::Utilities::generateRandomInteger(min, max);
    ASSERT_GE(result, min);
    ASSERT_LE(result, max);
}

TEST_F(UtilityTest, GenerateRandomIntegerReturnsValueWithinFullRange) {
    int result = Modbus::Utilities::generateRandomInteger();
    ASSERT_GE(result, INT_MIN);
    ASSERT_LE(result, INT_MAX);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}