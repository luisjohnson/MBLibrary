cmake_minimum_required(VERSION 3.27)
project(MBLibrary)

set(CMAKE_CXX_STANDARD 20)


# Google Test
enable_testing()
add_subdirectory(external/googletest)
include_directories(${GTEST_INCLUDE_DIRS})

find_package(boost REQUIRED)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
else ()
    message(STATUS "Boost found")
    include_directories(${Boost_INCLUDE_DIRS})
    add_library(MBLibrary STATIC
            src/Modbus.cpp
            src/Modbus.h
            src/PDU.cpp
            src/PDU.h
            src/ModbusDataArea.cpp
            src/ModbusDataArea.h
            src/ModbusUtilities.cpp
            src/ModbusUtilities.h
            src/Logger.h
            src/Server.cpp
            src/Server.h)
    target_include_directories(MBLibrary PUBLIC src ${BOOST_LIBRARIES})

    add_executable(runTests tests/pduTests.cpp)
    target_link_libraries(runTests gtest gtest_main MBLibrary)

    add_executable(runModbusTests tests/modbusTests.cpp)
    target_link_libraries(runModbusTests gtest gtest_main MBLibrary)

    add_executable(runDataMapTests tests/dataMapTests.cpp)
    target_link_libraries(runDataMapTests gtest gtest_main MBLibrary)

    add_executable(runPDUTests tests/pduTests.cpp)
    target_link_libraries(runPDUTests gtest gtest_main MBLibrary)

    add_executable(runUtilityTests tests/utilityTests.cpp)
    target_link_libraries(runUtilityTests gtest gtest_main MBLibrary)
endif ()


