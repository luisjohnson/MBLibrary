#include <string>
#include "Modbus.h"
#include <iomanip>
#include <sstream>


bool isValidModbusFunctionCode(ModbusFunctionCode code) {
    switch (code) {
        case ModbusFunctionCode::ReadDiscreteInputs:
        case ModbusFunctionCode::ReadCoils:
        case ModbusFunctionCode::WriteSingleCoil:
        case ModbusFunctionCode::WriteSingleRegister:
        case ModbusFunctionCode::WriteMultipleCoils:
        case ModbusFunctionCode::ReadInputRegister:
        case ModbusFunctionCode::ReadHoldingRegisters:
        case ModbusFunctionCode::WriteMultipleRegisters:
        case ModbusFunctionCode::ReadWriteMultipleRegisters:
        case ModbusFunctionCode::ReadFifoQueue:
        case ModbusFunctionCode::ReadFileRecord:
        case ModbusFunctionCode::WriteFileRecord:
        case ModbusFunctionCode::ReadExceptionStatus:
        case ModbusFunctionCode::Diagnostic:
        case ModbusFunctionCode::GetComEventCounter:
        case ModbusFunctionCode::GetComEventLog:
        case ModbusFunctionCode::ReportSlaveID:
        case ModbusFunctionCode::ReadDeviceIdentification:
        default:
            return false;
    }
}

std::string fillWithZeros(const int value, const int length) {
    if (value < 0) {
        throw std::invalid_argument("Value must be non-negative");
    }
    std::ostringstream oss;
    oss << std::setw(length) << std::setfill('0') << value;
    return oss.str();
}

ModbusFunctionCode byteToModbusFunctionCode(std::byte b) {
    auto const code = static_cast<ModbusFunctionCode>(b);
    if (!isValidModbusFunctionCode(code))
        throw InvalidFunctionCodeException();
    return code;
}

int calculateBytesFromBits(int numberOfBits) {
    const int BitsPerByte = 8;
    int additionalBitsForUnevenByte = (numberOfBits % BitsPerByte) == 0 ? 0 : 1;
    return (numberOfBits / BitsPerByte) + additionalBitsForUnevenByte;
}

ModbusCoil::ModbusCoil(const int address, bool value) {
    _address = address;
    _value = value;
    _prefix = "0";
}

bool ModbusCoil::read() {
    return _value;
}

void ModbusCoil::write(bool value) {
    _value = value;
}

ModbusDiscreteInput::ModbusDiscreteInput(int address, bool value) {
    _address = address;
    _value = value;
    _prefix = "1";
}

bool ModbusDiscreteInput::read() {
    return _value;
}

void ModbusDiscreteInput::write(bool value) {
    _value = value;
}

ModbusInputRegister::ModbusInputRegister(int address, uint16_t value) {
    _address = address;
    _value = value;
    _prefix = "3";
}

uint16_t ModbusInputRegister::read() {
    return _value;
}

void ModbusInputRegister::write(uint16_t value) {
    _value = value;
}

ModbusHoldingRegister::ModbusHoldingRegister(int address, uint16_t value) {
    _address = address;
    _value = value;
    _prefix = "4";
}

uint16_t ModbusHoldingRegister::read() {
    return _value;
}

void ModbusHoldingRegister::write(uint16_t value) {
    _value = value;
}
