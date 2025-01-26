cmake_minimum_required(VERSION 3.10)
project(MBLibrary)

set(CMAKE_CXX_STANDARD 20)

# Set policy for CMP0167
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

# Google Test
#enable_testing()
find_package(GTest REQUIRED)
#find_package(GMock REQUIRED)
#add_subdirectory(external/googletest)
include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})


find_package(Boost)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
else ()
    message(STATUS "Boost found")
    add_library(MBLibrary STATIC
            src/Modbus.cpp
            src/Modbus.h
            src/ModbusPDU.cpp
            src/ModbusPDU.h
            src/ModbusDataArea.cpp
            src/ModbusDataArea.h
            src/ModbusUtilities.cpp
            src/ModbusUtilities.h
            src/ModbusServer.cpp
            src/ModbusServer.h
            src/ModbusClient.cpp
            src/ModbusClient.h
    )
    target_include_directories(MBLibrary PUBLIC src ${Boost_INCLUDE_DIRS})
    target_link_libraries(MBLibrary ${Boost_LIBRARIES})

    add_executable(runTests tests/pduTests.cpp)
    target_link_libraries(runTests GTest::GTest GTest::Main MBLibrary ${Boost_LIBRARIES})

    add_executable(runModbusTests tests/modbusTests.cpp)
    target_link_libraries(runModbusTests GTest::GTest GTest::Main MBLibrary)

    add_executable(runDataMapTests tests/dataMapTests.cpp)
    target_link_libraries(runDataMapTests GTest::GTest GTest::Main MBLibrary)

    add_executable(runPDUTests tests/pduTests.cpp)
    target_link_libraries(runPDUTests GTest::GTest GTest::Main MBLibrary)

    add_executable(runUtilityTests tests/utilityTests.cpp)
    target_link_libraries(runUtilityTests GTest::GTest GTest::Main  MBLibrary)

    add_executable(runClientTests tests/clientTests.cpp)
    target_link_libraries(runClientTests GTest::GTest GTest::Main GTest::gmock GTest::gmock MBLibrary)

    add_executable(ServerDemo demos/server/main.cpp)
    target_link_libraries(ServerDemo MBLibrary)

endif ()


