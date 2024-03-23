#include <gtest/gtest.h>
#include <ModbusDataArea.h>
#include <Modbus.h>
#include <ModbusPDU.h>
#include <memory>

class ModbusPDUTest : public ::testing::Test {
protected:

    std::shared_ptr<ModbusDataArea> modbusDataArea = std::make_shared<ModbusDataArea>();


    void SetUp() override {
        auto coil = ModbusCoil(0, true);
        for (int i = 0; i < 10; ++i) {
            modbusDataArea->insertCoil(ModbusCoil (i, i % 2 == 0));
        }
        modbusDataArea->insertCoil(coil);
    }
};

TEST_F(ModbusPDUTest, ReadCoilsResponseReturnsCorrectData) {

    ModbusPDU pdu(ModbusFunctionCode::ReadCoils,
                  {std::byte{0x00}, std::byte{0x01}, std::byte{0x00}, std::byte{0x0A}},
                  modbusDataArea);

    auto response = pdu.buildResponse();

    ASSERT_EQ(response.size(), 4);
//    ASSERT_EQ(response[0], std::byte{0x01});
//    ASSERT_EQ(response[1], std::byte{0x02});
//    ASSERT_EQ(response[2], std::byte{0b01010101});
//    ASSERT_EQ(response[3], std::byte{0b00000001});
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
