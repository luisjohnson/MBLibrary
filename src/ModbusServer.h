//
// Created by ljohnson on 3/28/2024.
//

#ifndef MBLIBRARY_MODBUSSERVER_H
#define MBLIBRARY_MODBUSSERVER_H

#include <boost/asio.hpp>
#include "Modbus.h"
#include "ModbusDataArea.h"

using boost::asio::ip::tcp;


namespace Modbus::Server {

    /**
     * @brief Starts a listener using Boost.Asio.
     *
     * This function starts a listener using Boost.Asio, which allows asynchronous
     * communication. It is implemented using the C++20 feature 'coroutines' and
     * Boost.Asio's co_awaitable framework.
     *
     * @return A Boost.Asio awaitable indicating the completion or failure of the listener.
     *     It has no return value.
     */
    boost::asio::awaitable<void> listener();

    /**
     * @brief Reads data from a TCP socket asynchronously using Boost.Asio.
     *
     * This function uses Boost.Asio's coroutines to perform asynchronous I/O operations.
     * It reads data from the given TCP socket and suspends the execution until the operation completes.
     * The function returns a Boost.Asio awaitable object that the caller can use to manage the asynchronous operation.
     * When the operation completes, the coroutine is asynchronously resumed.
     *
     * @param socket The TCP socket to read data from.
     *
     * @return A Boost.Asio awaitable object that represents the asynchronous read operation.
     */
    boost::asio::awaitable<void> read_data(tcp::socket socket);

    /**
     * @class Server
     * @brief Represents a server that listens for incoming Modbus requests
     *
     * The Server class provides methods to start and stop the server,
     * as well as handling incoming connections and processing Modbus requests.
     */
    class Server {
    public:
        explicit Server(Modbus::DataArea &dataArea);

        /**
         * @fn void start()
         * @brief Starts the server and begins listening for incoming Modbus requests.
         *
         * This function starts the server by initializing the acceptor and binding it to a specific port.
         * It then enters an infinite loop, continuously accepting incoming connections and spawning sessions for each connection.
         * The acceptor is configured to use the provided DataArea object to process the Modbus requests.
         */
        void start();

        /**
         * @fn void stop()
         * @brief Stops the Modbus server.
         *
         * This function closes the acceptor, stopping the server from listening for incoming connections.
         */
        void stop();


    private:
        boost::asio::io_context _ioContext;

        short _port = 502;

        tcp::acceptor _acceptor{_ioContext, tcp::endpoint(tcp::v4(), _port)};

        Modbus::DataArea &_modbusDataArea;

        /**
         * @fn boost::asio::awaitable<void> listener()
         * @brief Listens for incoming connections and spawns a session for each connection.
         *
         * This function is responsible for accepting incoming connections and spawning a session for each connection.
         * It executes an infinite loop, continuously accepting new connections and handling them in separate sessions.
         */
        boost::asio::awaitable<void> listener();

        /**
        * @fn boost::asio::awaitable<void> session(tcp::socket socket)
        * @brief Handles a session with a connected client.
        *
        * This function is responsible for managing a session with a client that has connected to the server.
        * It reads data from the client, processes the Modbus request, and sends a response back to the client.
        * The function is implemented using Boost.Asio's coroutines, which allows it to perform asynchronous I/O operations.
        * The function returns a Boost.Asio awaitable object that the caller can use to manage the asynchronous operation.
        *
        * @param socket The TCP socket associated with the client.
        *
        * @return A Boost.Asio awaitable object that represents the asynchronous session handling operation.
        */
        boost::asio::awaitable<void> session(tcp::socket socket);

    };
}


#endif //MBLIBRARY_MODBUSSERVER_H
