enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(runTests pduTests.cpp)
target_link_libraries(runTests gtest_main MBLibrary ${Boost_LIBRARIES})

#add_executable(runModbusTests modbusTests.cpp)
#target_link_libraries(runModbusTests gtest_main MBLibrary)

add_executable(runDataMapTests dataMapTests.cpp)
target_link_libraries(runDataMapTests gtest gtest_main MBLibrary)

add_executable(runPDUTests pduTests.cpp)
target_link_libraries(runPDUTests gtest gtest_main MBLibrary)

add_executable(runUtilityTests utilityTests.cpp)
target_link_libraries(runUtilityTests gtest gtest_main MBLibrary)

#add_executable(runClientTests clientTests.cpp)
#target_link_libraries(runClientTests gtest gtest_main GTest::gmock GTest::gmock MBLibrary)
