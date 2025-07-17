//
// Created by ljohnson on 4/4/2024.
//

#ifndef MBLIBRARY_MODBUSCLIENT_H
#define MBLIBRARY_MODBUSCLIENT_H

#include <vector>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include "Modbus.h"

namespace Modbus {
    class Client {
    public:
        explicit Client(std::string ip, int port = 502);

        boost::asio::awaitable<void> connect(); // TODO: Implement the method to establish a connection to the Modbus server

        void disconnect(); // TODO: Implement the method to disconnect from the Modbus server

        std::vector<bool> readCoils(uint16_t startAddress, uint16_t quantity); // TODO: Implement the method to read coils from the Modbus server

        std::vector<bool> readDiscreteInputs(uint16_t startAddress, uint16_t quantity); // TODO: Implement the method to read discrete inputs from the Modbus server

        std::vector<uint16_t> readHoldingRegisters(uint16_t startAddress, uint16_t quantity); // TODO: Implement the method to read holding registers from the Modbus server

        std::vector<uint16_t> readInputRegisters(uint16_t startAddress, uint16_t quantity); // TODO: Implement the method to read input registers from the Modbus server

        void writeSingleCoil(uint16_t address, bool value); // TODO: Implement the method to write a single coil to the Modbus server

        void writeSingleRegister(uint16_t address, uint16_t value); // TODO: Implement the method to write a single register to the Modbus server

        void writeMultipleCoils(uint16_t startAddress, uint16_t quantity, const std::vector<bool> &values); // TODO: Implement the method to write multiple coils to the Modbus server

        void writeMultipleRegisters(uint16_t startAddress, uint16_t quantity, const std::vector<uint16_t> &values); // TODO: Implement the method to write multiple registers to the Modbus server

    private:
        boost::asio::io_context _ioContext; 
        boost::asio::ip::tcp::socket _socket; 
        std::string _ip; 
        int _port;

        boost::asio::awaitable<std::vector<std::byte>> requestDataFromServer(const std::vector<std::byte> &requestRawData); // TODO: Implement the method to send a request to the Modbus server and receive the response

        bool isRequestValid(const std::vector<std::byte> &requestRawData, FunctionCode functionCode); // TODO: Implement the method to validate the request data
    };;


#endif //MBLIBRARY_MODBUSCLIENT_H
}