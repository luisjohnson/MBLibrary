//
// Created by ljohnson on 3/28/2024.
//

#include <cstddef>
#include <vector>
#include "ModbusServer.h"
#include "ModbusPDU.h"
#include <iostream>

Modbus::Server::Server::Server(Modbus::DataArea &dataArea) : _modbusDataArea(dataArea) {
    _ioContext.run();
}

void Modbus::Server::Server::start() {
    boost::asio::co_spawn(_acceptor.get_executor(), [this]() { return listener(); },
                          boost::asio::detached);
}

void Modbus::Server::Server::stop() {
    _acceptor.close();
    _ioContext.stop();
}

boost::asio::awaitable<void> Modbus::Server::Server::listener() {
    for (;;) {
        tcp::socket socket = co_await _acceptor.async_accept(boost::asio::use_awaitable);
        boost::asio::co_spawn(_acceptor.get_executor(),
                              [this, socket = std::move(socket)]() mutable { return session(std::move(socket)); },
                              boost::asio::detached);
    }
}

boost::asio::awaitable<void> Modbus::Server::Server::session(tcp::socket socket) {
    try {
        for (;;) {
            std::array<std::byte, 1024> data{};
            std::size_t n = co_await socket.async_read_some(boost::asio::buffer(data),
                                                            boost::asio::use_awaitable);
            std::cout << "Received " << n << " bytes" << std::endl;
//            std::vector<std::byte> bytes(data.size());
//            std::copy(data.begin(), data.end(), bytes.begin());
//            Modbus::PDU pdu(bytes, _modbusDataArea);
//            auto response = pdu.buildResponse();

//            co_await boost::asio::async_write(socket, boost::asio::buffer(response, n),
//                                              boost::asio::use_awaitable);
        }
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
//        Logger::error(e.what());
    }
}