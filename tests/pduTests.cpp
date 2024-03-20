#include <gtest/gtest.h>
#include <cstddef>
#include <Modbus.h>
#include <ModbusPDU.h>

class ModbusTest : public ::testing::Test {
protected:
    std::vector<std::shared_ptr<ModbusCoil>> coils;
    std::vector<std::shared_ptr<ModbusDiscreteInput>> discreteInputs;

    void SetUp() override {
        // Initialize the coils and discrete inputs
        for (int i = 0; i < 10; ++i) {
            coils.push_back(std::make_shared<ModbusCoil>(i, i % 2 == 0));
            discreteInputs.push_back(std::make_shared<ModbusDiscreteInput>(i, i % 2 == 0));
        }
    }
};

TEST(calculateBytesFromBits_Test, TestWithEvenBits) {
    ASSERT_EQ(calculateBytesFromBits(16), 2);
}

TEST(calculateBytesFromBits_Test, TestWithOddBits) {
    ASSERT_EQ(calculateBytesFromBits(17), 3);
}

TEST(calculateBytesFromBits_Test, TestWithZeroBits) {
    ASSERT_EQ(calculateBytesFromBits(0), 0);
}

TEST(calculateBytesFromBits_Test, TestWithBitsLessThanEight) {
    ASSERT_EQ(calculateBytesFromBits(4), 1);
}

TEST_F(ModbusTest, packBooleanRegistersIntoBytes_Coils) {
    auto packedCoils = packBooleanRegistersIntoBytes(coils);

    // Check the size of the packed coils
    EXPECT_EQ(packedCoils.size(), 2);

    // Check the values of the packed coils
    EXPECT_EQ(packedCoils[0], std::byte{0b01010101});
    EXPECT_EQ(packedCoils[1], std::byte{0b00000001});
}

TEST_F(ModbusTest, packBooleanRegistersIntoBytes_DiscreteInputs) {
    auto packedDiscreteInputs = packBooleanRegistersIntoBytes(discreteInputs);

    // Check the size of the packed discrete inputs
    EXPECT_EQ(packedDiscreteInputs.size(), 2);

    // Check the values of the packed discrete inputs
    EXPECT_EQ(packedDiscreteInputs[0], std::byte{0b01010101});
    EXPECT_EQ(packedDiscreteInputs[1], std::byte{0b00000001});
}

TEST_F(ModbusTest, packBooleanRegistersIntoBytes_EmptyVector) {
    std::vector<std::shared_ptr<ModbusCoil>> emptyCoils;
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
