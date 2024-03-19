cmake_minimum_required(VERSION 3.27)
project(MBLibrary)

set(CMAKE_CXX_STANDARD 20)

# Google Test
enable_testing()
add_subdirectory(external/googletest)
include_directories(${GTEST_INCLUDE_DIRS})


add_library(MBLibrary STATIC
        src/Modbus.cpp
        src/Modbus.h
        src/ModbusPDU.cpp
        src/ModbusPDU.h
        src/ModbusDataArea.cpp
        src/ModbusDataArea.h
        src/ModbusPDUFactory.cpp
        src/ModbusPDUFactory.h)
target_include_directories(MBLibrary PUBLIC src)

add_executable(runTests tests/pduTests.cpp)
target_link_libraries(runTests gtest gtest_main MBLibrary)

add_executable(runModbusTests tests/modbusTests.cpp)
target_link_libraries(runModbusTests gtest gtest_main MBLibrary)

add_executable(runDataMapTests tests/dataMapTests.cpp)
target_link_libraries(runDataMapTests gtest gtest_main MBLibrary)