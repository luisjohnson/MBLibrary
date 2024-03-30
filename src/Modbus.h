//
// Created by Luis Johnson on 3/10/24.
//

#ifndef MODBUS_H
#define MODBUS_H

#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdint>
#include <ranges>
#include <exception>
#include <memory>

namespace Modbus {
    /**
        * @enum FunctionCode
        * @brief Enumeration of Modbus function codes.
        *
        * This enumeration represents the function codes used in Modbus protocol.
        * Each function code corresponds to a specific operation that can be performed
        * on a Modbus device.
        *
        * The FunctionCode enumeration provides a set of named constants that
        * can be used to specify the function code to be used in Modbus requests and
        * responses.
        *
        * The names of the constants are self-explanatory and follow the naming
        * convention used in the Modbus protocol specification.
        *
        * @par Example
        * @code{.cpp}
        * auto functionCode = FunctionCode::ReadCoils;
        * @endcode
        *
        */
    enum class FunctionCode {
        ReadCoils = 0x01,
        ReadDiscreteInputs = 0x02,
        ReadHoldingRegisters = 0x03,
        ReadInputRegister = 0x04,
        WriteSingleCoil = 0x05,
        WriteSingleRegister = 0x06,
        ReadExceptionStatus = 0x07,
        Diagnostic = 0x08,
        WriteMultipleRegisters = 0x10,
        ReportSlaveID = 0x11,
        ReadFileRecord = 0x14,
        WriteFileRecord = 0x15,
        GetComEventCounter = 0x0B,
        GetComEventLog = 0x0C,
        WriteMultipleCoils = 0x0F,
        ReadWriteMultipleRegisters = 0x17,
        ReadFifoQueue = 0x18,
        ReadDeviceIdentification = 0x2B
    };

    /**
     * @enum ExceptionCode
     * @brief Enumeration of Modbus exception codes.
     *
     * This enumeration represents the exception codes used in Modbus protocol.
     * Each exception code corresponds to a specific error condition that can occur
     * during the execution of a Modbus request.
     *
     * The ExceptionCode enumeration provides a set of named constants that
     * can be used to specify the exception code in Modbus responses.
     *
     * The names of the constants are self-explanatory and follow the naming
     * convention used in the Modbus protocol specification.
     *
     * @par Example
     * @code{.cpp}
     * auto exceptionCode = ExceptionCode::IllegalFunction;
     * @endcode
     */
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
     * @brief Converts a Modbus function code to a byte.
     *
     * This function converts a Modbus function code to a byte.
     * The function code is represented as an enumeration value of the FunctionCode
     * enumeration.
     *
     * @param functionCode The Modbus function code to convert.
     * @return The Modbus function code as a byte.
     *
     * @par Example
     * @code{.cpp}
     * auto functionCode = FunctionCode::ReadCoils;
     * auto byte = modbusFunctionCodeToByte(functionCode);
     * @endcode
     */
    FunctionCode byteToModbusFunctionCode(std::byte b);

//    /**
//         * @class InvalidFunctionCodeException
//         * @brief An exception class for indicating an invalid Modbus function code.
//         *
//         * This exception is thrown when an invalid Modbus function code is provided.
//         * It inherits from std::exception and overrides the what() method to provide
//         * a description of the exception.
//         */
//    class InvalidFunctionCodeException : std::exception {
//    public:
//        const char *what() const noexcept override {
//            return "Invalid Modbus function code provided.";
//        }
//    };
//
//    class RegisterExistsException : std::exception {
//    public:
//        const char *what() const noexcept override {
//            return "Register already exists.";
//        }
//    };

    /**
     * @brief Fill the given value with zeros until it reaches the specified length.
     *
     * This function takes an integer value and a length as input. It fills the value with leading zeros until it reaches
     * the specified length. The result is returned as a string.
     *
     * @param value The value to be filled with zeros.
     * @param length The desired length of the resulting string.
     * @return The value filled with zeros as a string.
     *
     * @throws std::invalid_argument If the value is negative.
     *
     * @note This function assumes a non-negative value is provided. If a negative value is passed, an exception of type
     * std::invalid_argument will be thrown. Make sure to handle this exception appropriately.
     * @par example
     * @code{.cpp}
     * auto result = fillWithZeros(123, 5); // returns "00123"
     * @endcode
     */
    std::string fillWithZeros(const int value, const int length);



    /**
     * @brief Calculates the number of bytes required to store a specified number of bits.
     *
     * This function takes in the number of bits and returns the corresponding number of bytes required to store those bits.
     *
     * @param numberOfBits The number of bits to be converted into bytes.
     * @return The number of bytes required to store the given number of bits.
     * @code{.cpp}
     * auto result = calculateBytesFromBits(16); // returns 2
     * @endcode
     */
    int calculateBytesFromBits(int numberOfBits);


    /**
     * @brief The Register class represents a modbus register.
     *
     * This class provides functionality to read and write values to a modbus register.
     * Each register has an address which can be accessed using the getAddress() method.
     * The value stored in the register can be accessed using the read() method, and can be modified using the write() method.
     *
     * @tparam T The data type of the register value.
     */
    template<typename T>
    class Register {
    public:

        virtual ~Register() = default;

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
         * Coil coil1(1, true);
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
         * Coil coil1(1, true);
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
         * Coil coil1(1, true);
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
         * Coil coil1(1, true);
         * int address = coil1.getAddress(); // returns 1
         * @endcode
         */
    template<typename T>
    int Register<T>::getAddress() const {
        return _address;
    }

    /**
         * @brief Get the address of the Register with a prefix.
         *        The address is filled with leading zeros to have a total length of 5 characters.
         *        If _useHexPrefix is true, the prefix will have an "x" appended to it.
         *
         * @return The address with prefix as a string.
         *
         * @par Example
         * @code{.cpp}
         * Coil coil1(1, true);
         * std::string address = coil1.getAddressWithPrefix(); // returns "COIL00001" for prefix = "COIL"
         * @endcode
         */
    template<typename T>
    std::string Register<T>::getAddressWithPrefix() const {
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
         * Coil coil1(1, true);
         * std::string address = coil1.getAddressWithHexPrefix(); // returns "0x00001" for hex prefix
         * @endcode
         */
    template<typename T>
    std::string Register<T>::getAddressWithHexPrefix() const {
        return _prefix + "x" + fillWithZeros(_address, 5);
    }

    class Coil final : public Register<bool> {
    public:
        /**
         * @class Coil
         * @brief Represents a Modbus coil.
         *
         * This class provides methods to read and write values to a Modbus coil.
         * A Modbus coil is a single bit in a Modbus device that can be read and written to.
         *
         * @par Example
         * @code{.cpp}
         * Coil coil1(1, true); // Create a Coil object and initialize it to true
         * @endcode
         */
        Coil(int address, bool value);

        /**
         * @brief Reads the value of the Coil.
         *
         * This function retrieves the value of the Coil and returns it.
         *
         * @return bool The value of the Coil.
         *
         * @par Example
         * @code{.cpp}
         * Coil coil1(1, true); // Create a Coil object and initialize it to true
         * bool coilValue = coil1.read(); // Returns: true
         * @endcode
         */
        bool read() override;

        /**
         * @brief Writes a boolean value to the Coil object.
         *
         * This function updates the value of the Coil object with the specified boolean value.
         *
         * @param value The boolean value to be written to the Coil object.
         *
         * @note This function overrides the write function from the base class.
         *
         * @par Example
         * @code{.cpp}
         * Coil coil1(1, true); // Create a Coil object and initialize it to true
         * coil1.write(false); // Updates the value of the coil to false
         * @endcode
         */
        void write(bool value) override;
    };

    class DiscreteInput final : public Register<bool> {
    public:
        /**
         * @class DiscreteInput
         * @brief Represents a discrete input in a Modbus protocol.
         *
         * This class encapsulates the functionality for reading and writing
         * values to a discrete input in a Modbus protocol.
         *
         * @par Example
         * @code{.cpp}
         * DiscreteInput discInput(2, false); // Create a DiscreteInput object and initialize it to false
         * @endcode
         */
        DiscreteInput(int address, bool value);

        /**
         * @brief Reads the value of the DiscreteInput.
         *
         * This function retrieves the value of the DiscreteInput and returns it.
         *
         * @return bool The value of the DiscreteInput.
         *
         * @par Example
         * @code{.cpp}
         * DiscreteInput discInput(2, false); // Create a DiscreteInput object and initialize it to false
         * bool discValue = discInput.read(); // Returns: false
         * @endcode
         */
        bool read() override;

        /**
         * @brief Writes a boolean value to the DiscreteInput object.
         *
         * This function updates the value of the DiscreteInput object with the specified boolean value.
         *
         * @param value The boolean value to be written to the DiscreteInput object.
         *
         * @note This function overrides the write function from the base class.
         *
         * @par Example
         * @code{.cpp}
         * DiscreteInput discInput(2, false); // Create a DiscreteInput object and initialize it to false
         * discInput.write(true); // Updates the value of the discrete input to true
         * @endcode
         */
        void write(bool value) override;
    };

    class InputRegister final : public Register<uint16_t> {
    public:
        /**
         * @class InputRegister
         *
         * Represents a Modbus Input Register.
         *
         * This class provides methods to read and write values to the Input Register.
         *
         * @par Example
         * @code{.cpp}
         * InputRegister inputReg(3, 2000); // Create a InputRegister object and initialize it to 2000
         * @endcode
         */
        InputRegister(int address, uint16_t value);

        /**
         * @brief Reads the value of the InputRegister.
         *
         * This function overrides the read() function of the base class and returns
         * the value stored in the InputRegister.
         *
         * @return The value stored in the InputRegister.
         *
         * @see InputRegister::read()
         *
         * @par Example
         * @code{.cpp}
         * InputRegister inputReg(3, 2000); // Create a InputRegister object and initialize it to 2000
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
         * InputRegister inputReg(3, 2000); // Create a InputRegister object and initialize it to 2000
         * inputReg.write(3000); // Updates the value of the input register to 3000
         * @endcode
         */
        void write(uint16_t value) override;
    };

    class HoldingRegister final : public Register<uint16_t> {
    public:
        /**
         * @class HoldingRegister
         *
         * Represents a Modbus Holding Register, which is a component of a Modbus device that stores a 16-bit value.
         *
         * @par Example
         * @code{.cpp}
         * HoldingRegister holdReg(4, 5000); // Create a HoldingRegister object and initialize it to 5000
         * @endcode
         */
        HoldingRegister(int address, uint16_t value);

        /**
         * @brief This function is used to read the value of a Modbus Holding Register.
         *
         * @return The value of the Modbus Holding Register.
         *
         * @par Example
         * @code{.cpp}
         * HoldingRegister holdReg(4, 5000); // Create a HoldingRegister object and initialize it to 5000
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
         * HoldingRegister holdReg(4, 5000); // Create a HoldingRegister object and initialize it to 5000
         * holdReg.write(6000); // Updates the value of the holding register to 6000
         * @endcode
         */
        void write(uint16_t value) override;
    };
}


#endif //MODBUS_H
