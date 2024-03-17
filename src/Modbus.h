//
// Created by Luis Johnson on 3/10/24.
//

#ifndef MODBUS_H
#define MODBUS_H

#include <vector>
#include <string>
#include <cstdint>
#include <ranges>

/**
* @enum ModbusFunctionCode
* @brief Enumeration of Modbus function codes.
*
* This enumeration represents the function codes used in Modbus protocol.
* Each function code corresponds to a specific operation that can be performed
* on a Modbus device.
*
* The ModbusFunctionCode enumeration provides a set of named constants that
* can be used to specify the function code to be used in Modbus requests and
* responses.
*
* The names of the constants are self-explanatory and follow the naming
* convention used in the Modbus protocol specification.
*
*/
enum class ModbusFunctionCode {
    ReadDiscreteInputs = 0x02,
    ReadCoils = 0x01,
    WriteSingleCoil = 0x05,
    WriteMultipleCoils = 0x0F,
    ReadInputRegister = 0x04,
    ReadHoldingRegisters = 0x03,
    WriteSingleRegister = 0x06,
    WriteMultipleRegisters = 0x10,
    ReadWriteMultipleRegisters = 0x17,
    ReadFifoQueue = 0x18,
    ReadFileRecord = 0x14,
    WriteFileRecord = 0x15,
    ReadExceptionStatus = 0x07,
    Diagnostic = 0x08,
    GetComEventCounter = 0x0B,
    GetComEventLog = 0x0C,
    ReportSlaveID = 0x11,
    ReadDeviceIdentification = 0x2B
};


enum class ExceptionCode {
    IllegalFunction = 0x01,
    IllegalDataAddress = 0x02,
    IllegalDataValue = 0x03,
    ServerDeviceFailure = 0x04,
    Acknowledge = 0x05,
    ServerDeviceBusy = 0x06,
    MemoryParityError = 0x08,
    GatewayPathUnavailable = 0x0A,
    GatewayTargetDeviceFailedToRespond = 0x0B
};

bool isValidModbusFunctionCode(std::byte b);

std::string fillWithZeros(int value, int length);

template<typename T>
class ModbusRegister {
public:
    virtual ~ModbusRegister() = default;

    virtual T read() = 0;

    virtual void write(T value) = 0;

    int getAddress() const;

    std::string getAddressWithPrefix() const;

    void useHexPrefix(bool value);

protected:
    std::string _prefix;
    int _address{};
    T _value;
    bool _useHexPrefix = false;
};

template<typename T>
int ModbusRegister<T>::getAddress() const {
    return _address;
}

template<typename T>
std::string ModbusRegister<T>::getAddressWithPrefix() const {
    return _prefix + (_useHexPrefix ? "x" : "") + fillWithZeros(_address, 5);
}

template<typename T>
void ModbusRegister<T>::useHexPrefix(bool value) {
    _useHexPrefix = value;
};

class ModbusCoil final : public ModbusRegister<bool> {
public:
    ModbusCoil(int address, bool value);

    bool read() override;

    void write(bool value) override;
};

class ModbusDiscreteInput final : public ModbusRegister<bool> {
public:
    ModbusDiscreteInput(int address, bool value);

    bool read() override;

    void write(bool value) override;
};

class ModbusInputRegister final : public ModbusRegister<uint16_t> {
public:
    ModbusInputRegister(int address, uint16_t value);

    uint16_t read() override;

    void write(uint16_t value) override;
};

class ModbusHoldingRegister final : public ModbusRegister<uint16_t> {
public:
    ModbusHoldingRegister(int address, uint16_t value);

    uint16_t read() override;

    void write(uint16_t value) override;
};

#endif //MODBUS_H
