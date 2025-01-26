#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/asio/awaitable.hpp>
#include "ModbusClient.h"


class MockModbusClient : public Modbus::Client {
public:
    MockModbusClient(const std::string &ip, int port) : Modbus::Client(ip, port) {
    }

    MOCK_METHOD(boost::asio::awaitable<void>, connect, (), ());
};


TEST(ModbusClientTest, ConnectSuccess) {
    MockModbusClient mockClient("127.0.0.1", 502);

    EXPECT_CALL(mockClient, connect()).Times(1);

    mockClient.connect();
}

TEST(ModbusClientTest, ConnectResolveFailure) {
    MockModbusClient mockClient("132.0.0.1.8", 502);

    EXPECT_CALL(mockClient, connect()).WillOnce(testing::Throw(std::runtime_error("Failed to resolve the IP address")));

    EXPECT_THROW(mockClient.connect(), std::runtime_error);
}

TEST(ModbusClientTest, ConnectFailure) {
    MockModbusClient mockClient("192.168.0.1", 502);

    EXPECT_CALL(mockClient, connect()).WillOnce(testing::Throw(std::runtime_error("Failed to connect to the server")));

    EXPECT_THROW(mockClient.connect(), std::runtime_error);
}
