//
// Created by ljohnson on 3/28/2024.
//

#ifndef MBLIBRARY_SERVER_H
#define MBLIBRARY_SERVER_H

#include <boost/asio.hpp>
#include "Modbus.h"
#include "ModbusDataArea.h"

using boost::asio::ip::tcp;


namespace Modbus::Server {

    boost::asio::awaitable<void> listener();

    boost::asio::awaitable<void> read_data(tcp::socket socket);

    class Server {
    public:
        Server(boost::asio::io_context &io_context, short port);

        void start();

        void stop();

    private:
        tcp::acceptor _acceptor;

        boost::asio::awaitable<void> listener();

        boost::asio::awaitable<void> session(tcp::socket socket);

        Modbus::DataArea _modbusDataArea;

    };
}


#endif //MBLIBRARY_SERVER_H
