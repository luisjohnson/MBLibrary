
#include "ModbusClient.h"

#include <utility>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_awaitable.hpp>

Modbus::Client::Client(std::string ip, std::string port) : _ip(std::move(ip)), _port(std::move(port)),
                                                           _socket(_ioContext) {

}

void Modbus::Client::connect() {

    boost::asio::co_spawn(_ioContext, [this]() -> boost::asio::awaitable<void> {
        boost::asio::ip::tcp::resolver resolver(_ioContext);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(_ip, _port);

        co_await boost::asio::async_connect(_socket, endpoints, boost::asio::use_awaitable);
    }, boost::asio::detached);
    _ioContext.run();
}

void Modbus::Client::disconnect() {
    _ioContext.stop();
}

std::vector<bool> Modbus::Client::readCoils(uint16_t startAddress, uint16_t quantity) {
    if (!_socket.is_open()) {
        throw std::runtime_error("Client is not connected to a server");
    }
    //Create the request
    std::vector<std::byte> request = {

    };

    //Send the request
    return std::vector<bool>();
}

std::vector<bool> Modbus::Client::readDiscreteInputs(uint16_t startAddress, uint16_t quantity) {
    if (!_socket.is_open()) {
        throw std::runtime_error("Client is not connected to a server");
    }

    //Create the request

    int length = calculateBytesFromBits(quantity);
    auto mbap = createMBAP(length);
    std::vector<std::byte> requestPDU = {
            static_cast<std::byte>(Modbus::FunctionCode::ReadDiscreteInputs),
            static_cast<std::byte>(startAddress >> 8),
            static_cast<std::byte>(startAddress & 0xFF),
            static_cast<std::byte>(quantity >> 8),
            static_cast<std::byte>(quantity & 0xFF)
    };

    std::vector<std::byte> request = MBAPToBytes(mbap);

    request.insert(request.end(), requestPDU.begin(), requestPDU.end());

    //Send the request
    std::vector<std::byte> response = co_await sendRequest(request);
    return getBooleanRegisters(quantity, response);
}

std::vector<uint16_t> Modbus::Client::readHoldingRegisters(uint16_t startAddress, uint16_t quantity) {
    return std::vector<uint16_t>();
}

std::vector<uint16_t> Modbus::Client::readInputRegisters(uint16_t startAddress, uint16_t quantity) {
    return std::vector<uint16_t>();
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

Modbus::MBAP Modbus::Client::createMBAP(uint16_t length) {
    MBAP mbap{};
    mbap.transactionIdentifier = 0;
    mbap.protocolIdentifier = 0;
    mbap.length = length;
    mbap.unitIdentifier = 0;
    return mbap;
}

boost::asio::awaitable<std::vector<std::byte>> Modbus::Client::sendRequest(const std::vector<std::byte> &request) {

    // Send the request
    co_await boost::asio::async_write(_socket, boost::asio::buffer(request), boost::asio::use_awaitable);

    // Receive the response from the server
    std::vector<std::byte> response;
    response.reserve(1024);
    std::size_t len = co_await _socket.async_read_some(boost::asio::buffer(response), boost::asio::use_awaitable);

    // Resize the response vector to the actual number of bytes received
    response.resize(len);

    co_return response;
}

std::vector<bool> Modbus::Client::getBooleanRegisters(int quantity, const std::vector<std::byte> &response) {
    std::vector<bool> result;
    result.reserve(quantity);

    for (int i = 0; i < quantity; ++i) {
        std::byte currentByte = response[response.size() - 2 + i / 8];
        bool bitValue = static_cast<bool>((static_cast<int>(currentByte) & (0x1 << (i % 8))) >> (i % 8));
        result.push_back(bitValue);
    }

    return result;
}
