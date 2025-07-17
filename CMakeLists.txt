cmake_minimum_required(VERSION 3.10)
project(MBLibrary)

set(CMAKE_CXX_STANDARD 20)

# Set policy for CMP0167
if (POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
    cmake_policy(SET CMP0135 NEW)
endif ()

find_package(Boost)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
else ()
    message(STATUS "Boost found")
    add_library(MBLibrary STATIC
            src/Modbus.cpp
            src/ModbusPDU.cpp
            src/ModbusDataArea.cpp
            src/ModbusUtilities.cpp
            src/ModbusServer.cpp
            src/ModbusClient.cpp
    )
    target_include_directories(MBLibrary PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
            ${Boost_INCLUDE_DIRS})
    target_link_libraries(MBLibrary ${Boost_LIBRARIES})

    add_executable(ServerDemo demos/server/main.cpp)
    target_link_libraries(ServerDemo MBLibrary)
    enable_testing()
    add_subdirectory(tests)

endif ()


