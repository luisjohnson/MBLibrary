#include <gtest/gtest.h>
#include <cstddef>
#include <Modbus.h>
#include <ModbusPDU.h>


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
