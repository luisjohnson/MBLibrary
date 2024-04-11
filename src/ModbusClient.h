//
// Created by ljohnson on 4/4/2024.
//

#ifndef MBLIBRARY_MODBUSCLIENT_H
#define MBLIBRARY_MODBUSCLIENT_H

#include <vector>
#include <boost/asio.hpp>
#include "Modbus.h"

namespace Modbus {
    class Client {
    public:
        explicit Client(std::string ip, int port = 502);

        void connect();

        void disconnect();

        std::vector<bool> readCoils(uint16_t startAddress, uint16_t quantity);

        std::vector<bool> readDiscreteInputs(uint16_t startAddress, uint16_t quantity);

        std::vector<uint16_t> readHoldingRegisters(uint16_t startAddress, uint16_t quantity);

        std::vector<uint16_t> readInputRegisters(uint16_t startAddress, uint16_t quantity);

        void writeSingleCoil(uint16_t address, bool value);

        void writeSingleRegister(uint16_t address, uint16_t value);

        void writeMultipleCoils(uint16_t startAddress, uint16_t quantity, const std::vector<bool> &values);

        void writeMultipleRegisters(uint16_t startAddress, uint16_t quantity, const std::vector<uint16_t> &values);

    private:
        boost::asio::io_context _ioContext;
        boost::asio::ip::tcp::socket _socket;
        std::string _ip;
        int _port;

        std::vector<std::byte> requestDataFromServer(const std::vector<std::byte> &requestRawData);

        bool isRequestValid(const std::vector<std::byte> &requestRawData, FunctionCode functionCode);
    };


#endif //MBLIBRARY_MODBUSCLIENT_H
}