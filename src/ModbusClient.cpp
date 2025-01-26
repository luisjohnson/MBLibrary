#include "ModbusClient.h"
#include "ModbusPDU.h"
#include "ModbusUtilities.h"
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>

Modbus::Client::Client(std::string ip, int port) : _ip(std::move(ip)), _port(port), _socket(_ioContext) {
}

boost::asio::awaitable<void> Modbus::Client::connect() {
    boost::asio::ip::tcp::resolver resolver(_ioContext);
    boost::system::error_code ec;

    const auto endpoints = co_await resolver.async_resolve(_ip, std::to_string(_port),
                                                           boost::asio::redirect_error(boost::asio::use_awaitable, ec));
    if (ec) {
        throw std::runtime_error("Failed to resolve the IP address");
    }

    co_await boost::asio::async_connect(_socket, endpoints,
                                        boost::asio::redirect_error(boost::asio::use_awaitable, ec));
    if (ec) {
        throw std::runtime_error("Failed to connect to the server");
    }
}

void Modbus::Client::disconnect() {
    _socket.close();
}

std::vector<bool> Modbus::Client::readCoils(uint16_t startAddress, uint16_t quantity)  {

    return {};
}

std::vector<bool> Modbus::Client::readDiscreteInputs(uint16_t startAddress, uint16_t quantity) {
    return {};
}

std::vector<uint16_t> Modbus::Client::readHoldingRegisters(uint16_t startAddress, uint16_t quantity) {
    return {};
}

std::vector<uint16_t> Modbus::Client::readInputRegisters(uint16_t startAddress, uint16_t quantity) {
    return {};
}

void Modbus::Client::writeSingleCoil(uint16_t address, bool value) {
}

void Modbus::Client::writeSingleRegister(uint16_t address, uint16_t value) {
}

void Modbus::Client::writeMultipleCoils(uint16_t startAddress, uint16_t quantity, const std::vector<bool> &values) {
}

void
Modbus::Client::writeMultipleRegisters(uint16_t startAddress, uint16_t quantity, const std::vector<uint16_t> &values) {
}


boost::asio::awaitable<std::vector<std::byte> > Modbus::Client::requestDataFromServer(
    const std::vector<std::byte> &requestRawData) {
    boost::system::error_code ec;

    // Send the request data
    co_await boost::asio::async_write(_socket, boost::asio::buffer(requestRawData),
                                      boost::asio::redirect_error(boost::asio::use_awaitable, ec));
    if (ec) {
        throw std::runtime_error("Failed to send request to the server");
    }

    // Read the response header (assuming a fixed header size, e.g., 5 bytes)
    std::array<std::byte, 5> responseHeader{};
    co_await boost::asio::async_read(_socket, boost::asio::buffer(responseHeader),
                                     boost::asio::redirect_error(boost::asio::use_awaitable, ec));
    if (ec) {
        throw std::runtime_error("Failed to read response header from the server");
    }

    // Determine the length of the remaining response data
    const auto responseLength = static_cast<std::size_t>(responseHeader[4]);

    // Read the remaining response data
    std::vector<std::byte> responseBody(responseLength);
    co_await boost::asio::async_read(_socket, boost::asio::buffer(responseBody),
                                     boost::asio::redirect_error(boost::asio::use_awaitable, ec));
    if (ec) {
        throw std::runtime_error("Failed to read response body from the server");
    }

    // Combine the header and body into a single response vector
    std::vector<std::byte> responseRawData(responseHeader.begin(), responseHeader.end());
    responseRawData.insert(responseRawData.end(), responseBody.begin(), responseBody.end());

    co_return responseRawData;
}
