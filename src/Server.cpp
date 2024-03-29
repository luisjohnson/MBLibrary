//
// Created by ljohnson on 3/28/2024.
//

#include <cstddef>
#include <vector>
#include "Server.h"

Modbus::Server::Server::Server(boost::asio::io_context &io_context, short port) :
        _acceptor(io_context, tcp::endpoint(tcp::v4(), port)), _modbusDataArea() {
}

void Modbus::Server::Server::start() {
    boost::asio::co_spawn(_acceptor.get_executor(), [this]() { return listener(); },
                          boost::asio::detached);
}

void Modbus::Server::Server::stop() {
    _acceptor.close();
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
            std::vector<std::byte> data;
            std::size_t n = co_await socket.async_read_some(boost::asio::buffer(data),
                                                            boost::asio::use_awaitable);
            co_await boost::asio::async_write(socket, boost::asio::buffer(data, n),
                                              boost::asio::use_awaitable);
        }
    } catch (std::exception &e) {

    }
}
