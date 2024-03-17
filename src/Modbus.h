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

/**
 * @brief Check if the given Modbus function code is valid.
 *
 * This function checks if the given Modbus function code is valid by comparing it
 * with a list of valid codes. The valid codes are defined in the ModbusFunctionCode
 * enumeration.
 *
 * @param b The Modbus function code to validate.
 * @return true if the Modbus function code is valid, false otherwise.
 */
bool isValidModbusFunctionCode(std::byte b);

/**
 * @brief Fills the given value with leading zeros to match the desired length.
 *
 * This function takes an integer value and a length as parameters. It converts the integer value to a string and then adds leading zeros to the string to match the desired length. If
* the length of the converted string is already equal to or greater than the desired length, it will just return the converted string without any modification.
 *
 * @param value The integer value to be filled with leading zeros.
 * @param length The desired length of the resulting string.
 * @return The input value as a string with leading zeros to match the desired length.
 *
 * @note The function assumes that the desired length is a positive integer.
 */
std::string fillWithZeros(int value, int length);

template<typename T>
class ModbusRegister {
public:
    virtual ~ModbusRegister() = default;

    virtual T read() = 0;

    virtual void write(T value) = 0;

    int getAddress() const;

    std::string getAddressWithPrefix() const;

    std::string getAddressWithHexPrefix() const;

protected:
    std::string _prefix;
    int _address{};
    T _value;
};

/**
 * @brief Returns the address of the Modbus register.
 *
 * @tparam T The data type of the Modbus register.
 * @return int The address of the Modbus register.
 */
template<typename T>
int ModbusRegister<T>::getAddress() const {
    return _address;
}

/**
 * @brief Get the address of the ModbusRegister with a prefix.
 *        The address is filled with leading zeros to have a total length of 5 characters.
 *        If _useHexPrefix is true, the prefix will have an "x" appended to it.
 *
 * @return The address with prefix as a string.
 */
template<typename T>
std::string ModbusRegister<T>::getAddressWithPrefix() const {
    return _prefix + fillWithZeros(_address, 5);
}

/**
 * @brief Retrieves the address with a hexadecimal prefix.
 *
 * This function returns the Modbus register's address, formatted as a string with a hexadecimal prefix. The address is obtained by concatenating the prefix stored in the `_prefix` member
* variable with the address converted to a string with leading zeros added to match the desired length, which is 5 in this case.
 *
 * @return The Modbus register's address formatted as a string with a hexadecimal prefix.
 */
template<typename T>
std::string ModbusRegister<T>::getAddressWithHexPrefix() const {
    return _prefix + "x" + fillWithZeros(_address, 5);
}

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
