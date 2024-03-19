//
// Created by Luis Johnson on 3/10/24.
//

#ifndef MODBUS_H
#define MODBUS_H

#include <vector>
#include <string>
#include <cstdint>
#include <ranges>
#include <exception>

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
* @par Example
* @code{.cpp}
* auto functionCode = ModbusFunctionCode::ReadCoils;
* @endcode
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
 *
 * @par Example
 * @code{.cpp}
 * bool validity = isValidModbusFunctionCode(static_cast<std::byte>(ModbusFunctionCode::ReadCoils));
 * @endcode
 */
bool isValidModbusFunctionCode(std::byte b);

/**
 * @class InvalidFunctionCodeException
 * @brief An exception class for indicating an invalid Modbus function code.
 *
 * This exception is thrown when an invalid Modbus function code is provided.
 * It inherits from std::exception and overrides the what() method to provide
 * a description of the exception.
 */
class InvalidFunctionCodeException : public std::exception {
public:
    const char* what() const throw() override {
        return "Invalid Modbus function code provided.";
    }
};

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
 *
 * @par Example
 * @code{.cpp}
 * std::string filled = fillWithZeros(89, 5);  // returns "00089"
 * @endcode
 */
std::string fillWithZeros(int value, int length);

template<typename T>
class ModbusRegister {
public:
    /**
     * @brief The ModbusRegister class represents a modbus register.
     *
     * This class provides functionality to read and write values to a modbus register.
     * Each register has an address which can be accessed using the getAddress() method.
     * The value stored in the register can be accessed using the read() method, and can be modified using the write() method.
     *
     * @par Example
     * @code{.cpp}
     * // <ModbusCoil> and <ModbusHoldingRegister> are both subclasses of <ModbusRegister>
     * ModbusCoil coil1(1, true);
     * ModbusHoldingRegister holdReg1(1, 1200);
     * @endcode
     */
    virtual ~ModbusRegister() = default;

    /**
     * @brief Reads the value of a Modbus register.
     *
     * This function reads the value of a Modbus register.
     * It is a pure virtual function, which means that it is
     * declared in an abstract base class (interface) and must
     * be implemented in the derived classes.
     *
     * @return The value of the Modbus register.
     */
    virtual T read() = 0;

    /**
     * @brief Pure virtual function to write a value to a Modbus register.
     * @tparam T The data type of the value to write.
     * @param value The value to write to the register.
     * @note This function needs to be implemented by derived classes.
     * @note The implementation should handle the writing operation to the specific Modbus register.
     */
    virtual void write(T value) = 0;

    /**
     * @brief Returns the address of the Modbus register.
     *
     * This function returns the address of the Modbus register. The address represents the location of the register
     * in the Modbus device's memory map.
     *
     * @return int The address of the Modbus register.
     *
     * @par Example
     * @code{.cpp}
     * ModbusCoil coil1(1, true);
     * int address = coil1.getAddress(); // returns 1
     * @endcode
     */
    int getAddress() const;

    /**
     * @brief Get the address with the given prefix.
     *
     * This method returns the address with the specified prefix.
     * The prefix can be any string that is a valid address prefix.
     *
     * @note This method does not modify the original address.
     *
     * @return std::string The address with the specified prefix.
     *
     * @par Example
     * @code{.cpp}
     * ModbusCoil coil1(1, true);
     * std::string address = coil1.getAddressWithPrefix(); // returns "COIL00001" for prefix = "COIL"
     * @endcode
     */
    std::string getAddressWithPrefix() const;

    /**
     * @brief Returns the address with a hex prefix.
     *
     * This function is used to get the address of an object with a hex prefix.
     * The address is a unique identifier representing the location of the object
     * in the memory.
     *
     * @return The address with a hex prefix.
     *
     * @par Example
     * @code{.cpp}
     * ModbusCoil coil1(1, true);
     * std::string address = coil1.getAddressWithHexPrefix(); // returns "0x00001" for hex prefix
     * @endcode
     */
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
 *
 * @par Example
 * @code{.cpp}
 * ModbusCoil coil1(1, true);
 * int address = coil1.getAddress(); // returns 1
 * @endcode
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
 *
 * @par Example
 * @code{.cpp}
 * ModbusCoil coil1(1, true);
 * std::string address = coil1.getAddressWithPrefix(); // returns "COIL00001" for prefix = "COIL"
 * @endcode
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
 *
 * @par Example
 * @code{.cpp}
 * ModbusCoil coil1(1, true);
 * std::string address = coil1.getAddressWithHexPrefix(); // returns "0x00001" for hex prefix
 * @endcode
 */
template<typename T>
std::string ModbusRegister<T>::getAddressWithHexPrefix() const {
    return _prefix + "x" + fillWithZeros(_address, 5);
}

class ModbusCoil final : public ModbusRegister<bool> {
public:
    /**
     * @brief Represents a Modbus coil.
     *
     * This class represents a Modbus coil with an address and a value.
     * It provides methods for reading and writing the value of the coil.
     *
     * @par Example
     * @code{.cpp}
     * ModbusCoil coil1(1, true); // Create a ModbusCoil object
     * @endcode
     */
    ModbusCoil(int address, bool value);

    /**
     * @brief Reads the value of the Modbus coil.
     *
     * This function is used to read the current value of the Modbus coil.
     *
     * @return true if the coil value is true, false otherwise.
     *
     * @par Example
     * @code{.cpp}
     * ModbusCoil coil1(1, true); // Create a ModbusCoil object
     * bool coilValue = coil1.read(); // Returns: true
     * @endcode
     */
    bool read() override;

    /**
     * @brief Writes a boolean value to the ModbusCoil.
     *
     * This function allows writing a boolean value to the ModbusCoil object. The value will be stored in the internal variable `_value` of type `T`.
     *
     * @param value The boolean value to be written to the ModbusCoil.
     *
     * @pre None.
     * @post The `_value` variable will be updated with the specified boolean `value`.
     *
     * @par Example
     * @code{.cpp}
     * ModbusCoil coil(1, true); // Create a ModbusCoil object and initialize it to true
     * coil.write(false); // Updates the value of the coil to false
     * @endcode
     */
    void write(bool value) override;
};

class ModbusDiscreteInput final : public ModbusRegister<bool> {
public:
    /**
     * @class ModbusDiscreteInput
     * @brief Represents a discrete input in a Modbus protocol.
     *
     * This class encapsulates the functionality for reading and writing
     * values to a discrete input in a Modbus protocol.
     *
     * @par Example
     * @code{.cpp}
     * ModbusDiscreteInput discInput(2, false); // Create a ModbusDiscreteInput object and initialize it to false
     * @endcode
     */
    ModbusDiscreteInput(int address, bool value);

    /**
     * @brief Reads the value of the ModbusDiscreteInput.
     *
     * This function retrieves the value of the ModbusDiscreteInput and returns it.
     *
     * @return bool The value of the ModbusDiscreteInput.
     *
     * @par Example
     * @code{.cpp}
     * ModbusDiscreteInput discInput(2, false); // Create a ModbusDiscreteInput object and initialize it to false
     * bool discValue = discInput.read(); // Returns: false
     * @endcode
     */
    bool read() override;

    /**
     * @brief Writes a boolean value to the ModbusDiscreteInput object.
     *
     * This function updates the value of the ModbusDiscreteInput object with the specified boolean value.
     *
     * @param value The boolean value to be written to the ModbusDiscreteInput object.
     *
     * @note This function overrides the write function from the base class.
     *
     * @par Example
     * @code{.cpp}
     * ModbusDiscreteInput discInput(2, false); // Create a ModbusDiscreteInput object and initialize it to false
     * discInput.write(true); // Updates the value of the discrete input to true
     * @endcode
     */
    void write(bool value) override;
};

class ModbusInputRegister final : public ModbusRegister<uint16_t> {
public:
    /**
     * @class ModbusInputRegister
     *
     * Represents a Modbus Input Register.
     *
     * This class provides methods to read and write values to the Input Register.
     *
     * @par Example
     * @code{.cpp}
     * ModbusInputRegister inputReg(3, 2000); // Create a ModbusInputRegister object and initialize it to 2000
     * @endcode
     */
    ModbusInputRegister(int address, uint16_t value);

    /**
     * @brief Reads the value of the ModbusInputRegister.
     *
     * This function overrides the read() function of the base class and returns
     * the value stored in the ModbusInputRegister.
     *
     * @return The value stored in the ModbusInputRegister.
     *
     * @see ModbusInputRegister::read()
     *
     * @par Example
     * @code{.cpp}
     * ModbusInputRegister inputReg(3, 2000); // Create a ModbusInputRegister object and initialize it to 2000
     * uint16_t inputRegValue = inputReg.read(); // Returns: 2000
     * @endcode
     */
    uint16_t read() override;

    /**
     * @brief Write a value to the Modbus input register.
     *
     * This function is used to write a specified value to the Modbus input register.
     *
     * @param value The value to be written to the input register.
     *
     * @par Example
     * @code{.cpp}
     * ModbusInputRegister inputReg(3, 2000); // Create a ModbusInputRegister object and initialize it to 2000
     * inputReg.write(3000); // Updates the value of the input register to 3000
     * @endcode
     */
    void write(uint16_t value) override;
};

class ModbusHoldingRegister final : public ModbusRegister<uint16_t> {
public:
    /**
     * @class ModbusHoldingRegister
     *
     * Represents a Modbus Holding Register, which is a component of a Modbus device that stores a 16-bit value.
     *
     * @par Example
     * @code{.cpp}
     * ModbusHoldingRegister holdReg(4, 5000); // Create a ModbusHoldingRegister object and initialize it to 5000
     * @endcode
     */
    ModbusHoldingRegister(int address, uint16_t value);

    /**
     * @brief This function is used to read the value of a Modbus Holding Register.
     *
     * @return The value of the Modbus Holding Register.
     *
     * @par Example
     * @code{.cpp}
     * ModbusHoldingRegister holdReg(4, 5000); // Create a ModbusHoldingRegister object and initialize it to 5000
     * uint16_t holdRegValue = holdReg.read(); // Returns: 5000
     * @endcode
     */
    uint16_t read() override;

    /**
     * @brief Writes a value to the Modbus Holding Register.
     *
     * This function overrides the write() function from the base class. It sets the value of the Modbus Holding Register to the specified value.
     *
     * @param value The value to write to the Modbus Holding Register.
     *
     * @par Example
     * @code{.cpp}
     * ModbusHoldingRegister holdReg(4, 5000); // Create a ModbusHoldingRegister object and initialize it to 5000
     * holdReg.write(6000); // Updates the value of the holding register to 6000
     * @endcode
     */
    void write(uint16_t value) override;
};

#endif //MODBUS_H