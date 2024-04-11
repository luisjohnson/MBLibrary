#include "ModbusClient.h"
#include "ModbusPDU.h"
#include "ModbusUtilities.h"

Modbus::Client::Client(std::string ip, int port) : _ip(std::move(ip)), _port(port), _socket(_ioContext) {
}

void Modbus::Client::connect() {
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(_ip), _port);
    _socket.connect(endpoint);
}

void Modbus::Client::disconnect() {

    _socket.close();
}

std::vector<bool> Modbus::Client::readCoils(uint16_t startAddress, uint16_t quantity) {
    auto [StartAddressMSB, StartAddressLSB] = Modbus::Utilities::uint16ToTwoBytes(startAddress);
    auto [QuantityMSB, QuantityLSB] = Modbus::Utilities::uint16ToTwoBytes(quantity);
    std::vector<std::byte> requestRawData = {static_cast<std::byte>(Modbus::FunctionCode::ReadCoils),
                                             StartAddressMSB, StartAddressLSB, QuantityMSB, QuantityLSB};
    auto getResponse = requestDataFromServer(requestRawData);
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

std::vector<std::byte> Modbus::Client::requestDataFromServer(const std::vector<std::byte> &requestRawData) {
    return {};
}



