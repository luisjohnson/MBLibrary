//
// Created by ljohnson on 3/28/2024.
//

#include <cstddef>
#include <vector>
#include "ModbusServer.h"
#include "ModbusPDU.h"
#include <iostream>

Modbus::Server::MBServer::MBServer(Modbus::DataArea &dataArea) : _modbusDataArea(dataArea) {
}

void Modbus::Server::MBServer::start() {
    boost::asio::co_spawn(_acceptor.get_executor(), [this]() { return listener(); },
                          boost::asio::detached);
    _ioContext.run();
}

void Modbus::Server::MBServer::stop() {
    _acceptor.close();
    _ioContext.stop();
}

boost::asio::awaitable<void> Modbus::Server::MBServer::listener() {
    for (;;) {
        tcp::socket socket = co_await _acceptor.async_accept(boost::asio::use_awaitable);
        boost::asio::co_spawn(_acceptor.get_executor(),
                              [this, socket = std::move(socket)]() mutable { return session(std::move(socket)); },
                              boost::asio::detached);
    }
}

boost::asio::awaitable<void> Modbus::Server::MBServer::session(tcp::socket socket) {
    try {
        for (;;) {
            std::array<std::byte, 1024> data{};
            // Read data from the socket
            if (!socket.is_open()) {
                std::cerr << "Socket is closed" << std::endl;
                break;
            }
            // Execute the async_read_some operation and handle possible exceptions
            std::size_t receivedBytes = 0;
            try {
                receivedBytes = co_await socket.async_read_some(boost::asio::buffer(data), boost::asio::use_awaitable);
            } catch (const boost::system::system_error &e) {
                std::cerr << "Error on async_read_some: " << e.what() << std::endl;
                break;
            }

            std::vector<std::byte> bytes(receivedBytes);
            std::copy(data.begin(), data.begin() + receivedBytes, bytes.begin());
            auto response = co_await createResponse(bytes);

            // Create a buffer from the response vector
            auto responseBuffer = boost::asio::buffer(response, response.size());

            // Execute the async_write operation and handle possible exceptions
            try {
                // Write the response to the socket
                co_await boost::asio::async_write(socket, responseBuffer, boost::asio::use_awaitable);
            } catch (const boost::system::system_error &e) {
                std::cerr << "Error on async_write: " << e.what() << std::endl;
                break;
            }

        }
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

boost::asio::awaitable<std::vector<std::byte>> Modbus::Server::MBServer::createResponse(std::vector<std::byte> &bytes) {
    auto requestMbpa = Modbus::bytesToMBAP(bytes);
    auto bytesForPDU = std::vector<std::byte>(bytes.begin() + Modbus::Server::MBAP_HEADER_LENGTH, bytes.end());
    Modbus::PDU pdu(bytesForPDU, _modbusDataArea);
    auto responsePdu = pdu.buildResponse();
    auto responseMbpa = Modbus::MBAP{requestMbpa.transactionIdentifier, requestMbpa.protocolIdentifier,
                                     static_cast<uint16_t>(responsePdu.size() + 1), requestMbpa.unitIdentifier};
    auto response = Modbus::MBAPToBytes(responseMbpa);
    response.insert(response.end(), responsePdu.begin(), responsePdu.end());
    co_return response;
}
