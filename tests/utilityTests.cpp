#include <gtest/gtest.h>
#include <cstddef>
#include <ModbusUtilities.h>

class UtilityTest : public ::testing::Test {

protected:
    std::vector<ModbusCoil> coils;
    std::vector<ModbusDiscreteInput> discreteInputs;
    std::byte msb{};
    std::byte lsb{};
    uint16_t expected{};

    void SetUp() override {


        // Initialize the coils and discrete inputs
        for (int i = 0; i < 10; ++i) {
            coils.emplace_back(i, i % 2 == 0);
            discreteInputs.emplace_back(i, i % 2 == 0);
        }
    }
};


TEST_F(UtilityTest, TestNormalValues) {
    msb = std::byte(0xAB);
    lsb = std::byte(0xCD);
    expected = 0xABCD;

    ASSERT_EQ(twoBytesToUint16(msb, lsb), expected);
}

TEST_F(UtilityTest, TestMaximumValues) {
    msb = std::byte(0xFF);
    lsb = std::byte(0xFF);
    expected = 0xFFFF;

    ASSERT_EQ(twoBytesToUint16(msb, lsb), expected);
}

TEST_F(UtilityTest, TestMinimumValues) {
    msb = std::byte(0x00);
    lsb = std::byte(0x00);
    expected = 0x0000;

    ASSERT_EQ(twoBytesToUint16(msb, lsb), expected);
}

TEST_F(UtilityTest, TestRandomValues) {
    msb = std::byte(0x1F);
    lsb = std::byte(0x3E);
    expected = 0x1F3E;

    ASSERT_EQ(twoBytesToUint16(msb, lsb), expected);
}

TEST_F(UtilityTest, packBooleanRegistersIntoBytes_Coils) {
    auto packedCoils = packBooleanRegistersIntoBytes(coils);

    // Check the size of the packed coils
    EXPECT_EQ(packedCoils.size(), 2);

    // Check the values of the packed coils
    EXPECT_EQ(packedCoils[0], std::byte{0b01010101});
    EXPECT_EQ(packedCoils[1], std::byte{0b00000001});
}

TEST_F(UtilityTest, packBooleanRegistersIntoBytes_DiscreteInputs) {
    auto packedDiscreteInputs = packBooleanRegistersIntoBytes(discreteInputs);

    // Check the size of the packed discrete inputs
    EXPECT_EQ(packedDiscreteInputs.size(), 2);

    // Check the values of the packed discrete inputs
    EXPECT_EQ(packedDiscreteInputs[0], std::byte{0b01010101});
    EXPECT_EQ(packedDiscreteInputs[1], std::byte{0b00000001});
}

TEST_F(UtilityTest, packBooleanRegistersIntoBytes_EmptyVector) {
    std::vector<ModbusCoil> emptyCoils;
    auto packedCoils = packBooleanRegistersIntoBytes(emptyCoils);

    // Check the size of the packed coils
    EXPECT_TRUE(packedCoils.empty());
}

// TEST(ModbusTest, packBooleanRegistersIntoBytes_WrongType) {
//     std::vector<std::shared_ptr<ModbusInputRegister>> inputRegisters;
//     inputRegisters.push_back(std::make_shared<ModbusInputRegister>(1, 1000));
//
//     // Check that the function throws a static_assert
//     EXPECT_ANY_THROW(packBooleanRegistersIntoBytes(inputRegisters));
// }

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}