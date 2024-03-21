//
// Created by ljohnson on 3/20/2024.
//

#ifndef MBLIBRARY_MODBUSUTILITIES_H
#define MBLIBRARY_MODBUSUTILITIES_H

#include <cstdint>
#include <cstddef>
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
uint16_t twoBytesToUint16(std::byte msb, std::byte lsb) {
    return (static_cast<uint16_t>(msb) << 8) | static_cast<uint16_t>(lsb);
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
//    static_assert(std::is_same<T, ModbusCoil>::value || std::is_same<T, ModbusDiscreteInput>::value,
//                  "packBooleanRegistersIntoBytes accept only objects of type ModbusCoil or ModbusDiscreteInput.");
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

#endif //MBLIBRARY_MODBUSUTILITIES_H
