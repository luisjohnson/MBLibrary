//
// Created by ljohnson on 3/20/2024.
//

#ifndef MBLIBRARY_MODBUSUTILITIES_H
#define MBLIBRARY_MODBUSUTILITIES_H

#include <cstdint>
#include <cstddef>
#include <random>
#include <Modbus.h>


/**
 * @brief Converts two bytes into a 16-bit unsigned integer.
 *
 * This function takes two bytes and combines them into a single 16-bit unsigned integer.
 * The most significant byte (MSB) is shifted 8 bits to the left and then combined with the
 * least significant byte (LSB) using the bitwise OR operator.
 *
 * Example usage:
 * @code
 * std::byte msb = std::byte(0xAB);
 * std::byte lsb = std::byte(0xCD);
 * uint16_t result = twoBytesToUint16(msb, lsb);
 * @endcode
 *
 * @param msb The most significant byte.
 * @param lsb The least significant byte.
 * @return The combined 16-bit unsigned integer.
 */
inline uint16_t twoBytesToUint16(std::byte msb, std::byte lsb) {
    return (static_cast<uint16_t>(msb) << 8) | static_cast<uint16_t>(lsb);
}


/**
 * @brief Generates a random boolean value.
 *
 * This function uses a random device as a source of entropy to seed
 * the random number generator. It then generates a random integer
 * between 0 and 1 using a uniform distribution and returns it as a
 * boolean value.
 *
 * @return A random boolean value (true or false).
 */
inline bool generateRandomBoolean() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(0, 1);
    return distribution(gen);
}

/**
 * @brief Generates a random integer within a specified range.
 *
 * This function utilizes the C++ standard library's random number generation facilities to generate a random integer
 * within a given range. By default, the range is from the minimum representable value of an int to the maximum
 * representable value of an int, inclusive.
 *
 * @param min The lower bound of the range (default: INT_MIN).
 * @param max The upper bound of the range (default: INT_MAX).
 *
 * @return A randomly generated integer within the specified range.
 *
 * @note The function automatically seeds the random number generator using std::random_device, guaranteeing that
 *       each run produces a different sequence of random numbers.
 */
inline int generateRandomInteger(int min = INT_MIN, int max = INT_MAX) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(gen);
}

/**
 * @brief Packs boolean Modbus registers into bytes.
 *
 * This function takes a vector of shared pointers to Modbus registers (either ModbusCoil or ModbusDiscreteInput) and packs their boolean values into bytes.
 * Each bit in the byte corresponds to the boolean value of a register. The function returns a vector of bytes.
 *
 * @tparam T The type of the Modbus registers. Must be either ModbusCoil or ModbusDiscreteInput.
 * @param registers A vector of shared pointers to the Modbus registers to be packed into bytes.
 * @return A vector of bytes representing the packed boolean values of the Modbus registers.
 *
 * @throws static_assert If the type T is not ModbusCoil or ModbusDiscreteInput.
 *
 * @par Example
 * @code{.cpp}
 * std::vector<std::shared_ptr<ModbusCoil>> coils;
 * // ... (initialize the coils)
 * std::vector<std::byte> packedCoils = packBooleanRegistersIntoBytes(coils);
 * @endcode
 */
template<typename T>
std::vector<std::byte> packBooleanRegistersIntoBytes(std::vector<T> &registers) {
    static_assert(std::is_same<T, ModbusCoil>::value || std::is_same<T, ModbusDiscreteInput>::value,
                  "packBooleanRegistersIntoBytes accepts only objects of type ModbusCoil or ModbusDiscreteInput.");
    auto numberOfBits = registers.size();
    auto byteCount = calculateBytesFromBits(numberOfBits);
    std::vector<std::byte> bytes(byteCount);

    int bitCounter = 0;
    int byteCounter = 0;

    for (auto &reg: registers) {
        auto status = static_cast<std::byte>(reg.read());
        bytes[byteCounter] = bytes[byteCounter] | (status << bitCounter);
        bitCounter++;
        if (bitCounter > 7) {
            bitCounter = 0;
            byteCounter++;
        }
    }
    return bytes;
}


/**
 * @brief Packs a vector of Modbus integer registers into a vector of bytes.
 *
 * This function takes a vector of Modbus integer registers and packs their values into
 * a vector of bytes. It supports registers of type ModbusHoldingRegister and ModbusInputRegister.
 * Each register is packed into two bytes, with the most significant byte first (big-endian).
 *
 * @tparam T The type of registers in the vector (should be ModbusHoldingRegister or ModbusInputRegister).
 * @param registers The vector of registers to pack into bytes.
 * @return A vector of bytes containing the packed values of the registers.
 *
 * @throws std::invalid_argument If the template parameter T is not ModbusHoldingRegister or ModbusInputRegister.
 *
 * @par Example
 * @code{.cpp}
 * std::vector<ModbusHoldingRegister> holdingRegisters = {ModbusHoldingRegister(0, 5000), ModbusHoldingRegister(1, 6000)};
 * std::vector<std::byte> packedBytes = packIntegerRegistersIntoBytes(holdingRegisters);
 * // packedBytes will contain {0x13, 0x88, 0x17, 0x70} (assuming the values of the registers are 5000 and 6000 respectively)
 * @endcode
 */
template<typename T>
std::vector<std::byte> packIntegerRegistersIntoBytes(std::vector<T> &registers) {
    static_assert(std::is_same<T, ModbusHoldingRegister>::value || std::is_same<T,
                          ModbusInputRegister>::value,
                  "packIntegerRegistersIntoBytes accepts only objects of type ModbusHoldingRegister "
                  "or ModbusInputRegister.");
//    auto byteCount = registers.size() * 2;
    std::vector<std::byte> bytes;
    for (auto &reg: registers) {
        auto lsb = static_cast<std::byte>(reg.read() & 0xFF);
        auto msb = static_cast<std::byte>((reg.read() >> 8) & 0xFF);
        bytes.push_back(msb);
        bytes.push_back(lsb);
    }
    return bytes;
}


#endif //MBLIBRARY_MODBUSUTILITIES_H
