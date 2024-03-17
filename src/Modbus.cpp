#include <string>
#include "Modbus.h"
#include <iomanip>
#include <sstream>

static const std::vector<ModbusFunctionCode> validCodes = {
    ModbusFunctionCode::ReadDiscreteInputs,
    ModbusFunctionCode::ReadCoils,
    ModbusFunctionCode::WriteSingleCoil,
    ModbusFunctionCode::WriteMultipleCoils,
    ModbusFunctionCode::ReadInputRegister,
    ModbusFunctionCode::ReadHoldingRegisters,
    ModbusFunctionCode::WriteSingleRegister,
    ModbusFunctionCode::WriteMultipleRegisters,
    ModbusFunctionCode::ReadWriteMultipleRegisters,
    ModbusFunctionCode::ReadFifoQueue,
    ModbusFunctionCode::ReadFileRecord,
    ModbusFunctionCode::WriteFileRecord,
    ModbusFunctionCode::ReadExceptionStatus,
    ModbusFunctionCode::Diagnostic,
    ModbusFunctionCode::GetComEventCounter,
    ModbusFunctionCode::GetComEventLog,
    ModbusFunctionCode::ReportSlaveID,
    ModbusFunctionCode::ReadDeviceIdentification
};

bool isValidModbusFunctionCode(std::byte b) {
    const auto code = static_cast<ModbusFunctionCode>(static_cast<uint8_t>(b));
    return std::ranges::find(validCodes, code) != validCodes.end();
}

std::string fillWithZeros(const int value, const int length) {
    if (value < 0) {
        throw std::invalid_argument("Value must be non-negative");
    }
    std::ostringstream oss;
    oss << std::setw(length) << std::setfill('0') << value;
    return oss.str();
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
