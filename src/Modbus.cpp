#include <string>
#include "Modbus.h"
#include <iomanip>
#include <sstream>


Modbus::Coil::Coil(const int address, bool value) {
    _address = address;
    _value = value;
    _prefix = "0";
}


void Modbus::Coil::write(bool value) {
    _value = value;
}

bool Modbus::Coil::read() {
    return _value;
}

Modbus::DiscreteInput::DiscreteInput(int address, bool value) {
    _address = address;
    _value = value;
    _prefix = "1";
}

bool Modbus::DiscreteInput::read() {
    return _value;
}

void Modbus::DiscreteInput::write(bool value) {
    _value = value;
}

Modbus::InputRegister::InputRegister(int address, uint16_t value) {
    _address = address;
    _value = value;
    _prefix = "3";
}

uint16_t Modbus::InputRegister::read() {
    return _value;
}

void Modbus::InputRegister::write(uint16_t value) {
    _value = value;
}

Modbus::HoldingRegister::HoldingRegister(int address, uint16_t value) {
    _address = address;
    _value = value;
    _prefix = "4";
}

uint16_t Modbus::HoldingRegister::read() {
    return _value;
}

void Modbus::HoldingRegister::write(uint16_t value) {
    _value = value;
}


std::string Modbus::fillWithZeros(const int value, const int length) {
    if (value < 0) {
        throw std::invalid_argument("Value must be non-negative");
    }
    std::ostringstream oss;
    oss << std::setw(length) << std::setfill('0') << value;
    return oss.str();
}

int Modbus::calculateBytesFromBits(int numberOfBits) {
    const int BitsPerByte = 8;
    int additionalBitsForUnevenByte = (numberOfBits % BitsPerByte) == 0 ? 0 : 1;
    return (numberOfBits / BitsPerByte) + additionalBitsForUnevenByte;
}

Modbus::FunctionCode Modbus::byteToModbusFunctionCode(std::byte b) {
    return static_cast<FunctionCode>(b);
}