//
// Created by ljohnson on 3/20/2024.
//

#ifndef MBLIBRARY_MODBUSUTILITIES_H
#define MBLIBRARY_MODBUSUTILITIES_H

#include <cstdint>
#include <cstddef>


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

#endif //MBLIBRARY_MODBUSUTILITIES_H
