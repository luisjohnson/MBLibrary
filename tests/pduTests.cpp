#include <gtest/gtest.h>
#include <cstddef>
#include <Modbus.h>
#include <ModbusPDU.h>

TEST(PDUTests, CreatePDUFromRawDataTest) {
    std::vector<std::byte> data = {
        static_cast<std::byte>(0x01), static_cast<std::byte>(0x02), static_cast<std::byte>(0x03),
        static_cast<std::byte>(0x04)};
    const auto pdu = ModbusPDU(data);
    ASSERT_EQ(pdu.functionCode(), ModbusFunctionCode::ReadCoils);
}


TEST(PDUTests, GetTrasnmitDataTest) {
    std::vector<std::byte> data = {
        static_cast<std::byte>(0x01), static_cast<std::byte>(0x02), static_cast<std::byte>(0x03),
        static_cast<std::byte>(0x04)};
    const auto pdu = ModbusPDU(data);
    ASSERT_TRUE(data == pdu.getCompletePDU());
}

TEST(PDUTests, CreatePDUFromFunctionCodeTest) {
    const auto pdu = ModbusPDU(ModbusFunctionCode::ReadCoils);
    ASSERT_EQ(pdu.functionCode(), ModbusFunctionCode::ReadCoils);
    ASSERT_EQ(pdu.size(), 2);
}

TEST(PDUTests, CreatePDUFromFunctionCodeAndDataTest) {
    const std::vector<std::byte> data = {
        static_cast<std::byte>(0x01), static_cast<std::byte>(0x02), static_cast<std::byte>(0x03),
        static_cast<std::byte>(0x04)};
    const auto pdu = ModbusPDU(ModbusFunctionCode::ReadCoils, data);
    ASSERT_EQ(pdu.functionCode(), ModbusFunctionCode::ReadCoils);
}

TEST(PDUTests, GetSizeTest) {
    const std::vector<std::byte> data = {
        static_cast<std::byte>(0x01), static_cast<std::byte>(0x02), static_cast<std::byte>(0x03),
        static_cast<std::byte>(0x04)};
    const auto pdu = ModbusPDU(ModbusFunctionCode::ReadCoils, data);
    ASSERT_EQ(pdu.size(), 5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
