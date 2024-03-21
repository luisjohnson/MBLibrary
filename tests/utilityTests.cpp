#include <gtest/gtest.h>
#include <cstddef>
#include <ModbusUtilities.h>



struct TwoBytesToUInt16Test : public ::testing::Test {
    std::byte msb{};
    std::byte lsb{};
    uint16_t expected{};
};

TEST_F(TwoBytesToUInt16Test, TestNormalValues) {
    msb = std::byte(0xAB);
    lsb = std::byte(0xCD);
    expected = 0xABCD;

    ASSERT_EQ(twoBytesToUint16(msb, lsb), expected);
}

TEST_F(TwoBytesToUInt16Test, TestMaximumValues) {
    msb = std::byte(0xFF);
    lsb = std::byte(0xFF);
    expected = 0xFFFF;

    ASSERT_EQ(twoBytesToUint16(msb, lsb), expected);
}

TEST_F(TwoBytesToUInt16Test, TestMinimumValues) {
    msb = std::byte(0x00);
    lsb = std::byte(0x00);
    expected = 0x0000;

    ASSERT_EQ(twoBytesToUint16(msb, lsb), expected);
}

TEST_F(TwoBytesToUInt16Test, TestRandomValues) {
    msb = std::byte(0x1F);
    lsb = std::byte(0x3E);
    expected = 0x1F3E;

    ASSERT_EQ(twoBytesToUint16(msb, lsb), expected);
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}