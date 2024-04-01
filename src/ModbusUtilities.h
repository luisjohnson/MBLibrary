//
// Created by ljohnson on 3/20/2024.
//

#ifndef MBLIBRARY_MODBUSUTILITIES_H
#define MBLIBRARY_MODBUSUTILITIES_H

#include <cstdint>
#include <cstddef>
#include <random>
#include <Modbus.h>

namespace Modbus::Utilities {
    /**
     * @brief Converts two bytes (most significant byte and least significant byte) into a 16-bit unsigned integer.
     *
     * This function takes two bytes, representing the most significant byte (MSB) and the least significant byte (LSB),
     * and combines them to form a 16-bit unsigned integer. The MSB is shifted left by 8 bits and then bitwise OR'ed with
     * the LSB, resulting in a 16-bit unsigned integer.
     *
     * @param msb The most significant byte.
     * @param lsb The least significant byte.
     *
     * @return The 16-bit unsigned integer formed by combining the MSB and LSB.
     * @par Example
     * @code
     * auto msb = std::byte{0x01};
     * auto lsb = std::byte{0x02};
     * auto result = Modbus::Utilities::twoBytesToUint16(msb, lsb);
     * // result = 0x0102
     * @endcode
     */
    uint16_t twoBytesToUint16(std::byte msb, std::byte lsb);

    /**
     * @brief Function to generate a random boolean value.
     *
     * @return True if a randomly generated value is 1, false if it is 0.
     *
     * This function uses the C++ random number generation library to generate
     * a random boolean value. It initializes a random number generator using
     * std::random_device as a seed and std::mt19937 as the generator engine.
     * It then creates a uniform integer distribution with a range of 0 to 1.
     * The function returns true if the randomly generated value is 1, and false
     * if the value is 0.
     * @par Example
     * @code
     * auto result = Modbus::Utilities::generateRandomBoolean();
     * @endcode
     */
    bool generateRandomBoolean();

    /**
         * @brief Generate a random integer within the specified range.
         *
         * This function generates a random integer using the Mersenne Twister algorithm
         * within the specified range [min, max].
         *
         * @param min The minimum value of the integer range (default is INT_MIN).
         * @param max The maximum value of the integer range (default is INT_MAX).
         *
         * @return A random integer within the specified range [min, max].
         *
         * @note The random seed is generated using std::random_device.
         *       The Mersenne Twister algorithm (std::mt19937) is used to generate the random value.
         *       The random value is generated using std::uniform_int_distribution.
         *
         * @par Example
         * @code
         * auto result = Modbus::Utilities::generateRandomInteger(0, 100);
         * @endcode
         */
    int generateRandomInteger(int min = INT_MIN, int max = INT_MAX);

    /**
     * @brief Converts a vector of bytes to a vector of booleans.
     *
     * This function takes a vector of bytes and converts each byte into a vector of booleans.
     * Each byte is decomposed into its individual bits, and each bit is then stored as a boolean value in the resulting vector.
     *
     * @param bytes The vector of bytes to convert.
     * @return A vector of booleans representing the bits of the input bytes.
     *
     * @note The input vector of bytes must be non-empty.
     * @note The size of the resulting vector of booleans will be equal to the size of the input vector of bytes multiplied by 8.
     * @note The resulting vector of booleans will have the bits from the input bytes stored in the same order.
     *       The bits from the first byte are stored first, followed by the bits from the second byte, and so on.
     * @Par Example
     * @code
     * std::vector<std::byte> bytes = {std::byte(0b10101010)};
     * auto booleans = Modbus::Utilities::bytesToBooleans(bytes);
     * // booleans = {false, true, false, true, false, true, false, true}
     * @endcode
     */
    std::vector<bool> bytesToBooleans(const std::vector<std::byte> &bytes);

    /**
     * @brief Packs a vector of boolean registers into a vector of bytes.
     *
     * This function takes a vector of boolean registers (either objects of type Coil or DiscreteInput) and packs their
     * values into a vector of bytes. Each boolean value is packed into a
     * single bit of a byte, with the first boolean value being packed into the least significant bit of the first byte.
     *
     * @tparam T The type of the boolean registers. Must be either Coil or DiscreteInput.
     * @param registers A vector of boolean registers to be packed.
     * @return A vector of bytes with the packed boolean values.
     *
     * @par Example
     * @code{.cpp}
     * std::vector<Coil> coils;
     * // Populate the coils vector with Coil objects
     * std::vector<std::byte> packedBytes = packBooleanRegistersIntoBytes(coils);
     * @endcode
     */
    template<typename T>
    std::vector<std::byte> packBooleanRegistersIntoBytes(std::vector<T> &registers);

    /**
     * @brief Packs a vector of integer registers into a byte vector.
     *
     * This template function takes a vector of integer registers (either HoldingRegisters or InputRegisters)
     * and packs their values into a byte vector. Each register value is split into two bytes: the most significant
     * byte (MSB) and the least significant byte (LSB). The MSB is appended to the byte vector first, followed
     * by the LSB.
     *
     * @tparam T The type of the registers in the vector. Must be either HoldingRegister or InputRegister.
     * @param registers The vector of integer registers to be packed into bytes.
     * @return A byte vector containing the packed register values.
     *
     * @par Example:
     *
     * Assuming we have the following HoldingRegister objects:
     *
     * HoldingRegister holdReg1(4, 5000); // value = 5000
     * HoldingRegister holdReg2(5, 10000); // value = 10000
     *
     * and we call the function with the vector {holdReg1, holdReg2}, the expected return value would be
     * a byte vector containing {0x13, 0x88, 0x27, 0x10}.
     *
     * In the above example, holdReg1 has a value of 5000, which is equivalent to 0x1388 in hexadecimal.
     * The MSB is 0x13 and the LSB is 0x88. Similarly, holdReg2 has a value of 10000,
     * which is equivalent to 0x2710 in hexadecimal.
     * The MSB is 0x27 and the LSB is 0x10. The byte vector is formed by appending the MSBs and LSBs of the registers in order.
     *
     * @warning The function throws a static_assert error if the type T is not HoldingRegister or InputRegister.
     */
    template<typename T>
    std::vector<std::byte> packIntegerRegistersIntoBytes(std::vector<T> &registers);

    /**
     * @brief Packs boolean registers into a vector of bytes.
     *
     * This function takes a vector of boolean registers (Coil or DiscreteInput objects) and packs them into a vector of bytes.
     * Each boolean register represents a single bit and is packed into the byte vector according to its position in the vector.
     * The function returns the packed byte vector.
     *
     * @tparam T The type of the registers in the input vector. Only objects of type Coil or DiscreteInput are accepted.
     * @param registers The vector of boolean registers to be packed into bytes.
     * @return std::vector<std::byte> The packed byte vector.
     *
     * @note The function throws static_assert error if the input vector contains registers of unsupported types.
     *
     * @par Example
     * @code{.cpp}
     * std::vector<Coil> registers;
     * // Fill the registers vector with Coil objects
     *
     * std::vector<std::byte> packedBytes = packBooleanRegistersIntoBytes(registers);
     * @endcode
     */
    template<typename T>
    std::vector<std::byte> packBooleanRegistersIntoBytes(std::vector<T> &registers) {
        static_assert(std::is_same<T, Coil>::value || std::is_same<T, DiscreteInput>::value,
                      "packBooleanRegistersIntoBytes accepts only objects of type Coil or DiscreteInput.");
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
     * @brief Packs a vector of integer registers into a vector of bytes.
     *
     * This function takes a vector of integer registers, either HoldingRegister or InputRegister objects, and packs their values into a vector of bytes. Each register value is split into
    * two bytes (MSB and LSB) and appended to the result vector.
     *
     * @tparam T The type of registers in the input vector.
     * @param registers The vector of integer registers to pack.
     * @return A vector of bytes containing the packed register values.
     *
     * @note This function accepts only objects of type HoldingRegister or InputRegister.
     *
     * @see HoldingRegister
     * @see InputRegister
     *
     * @par Example
     * @code{.cpp}
     * std::vector<HoldingRegister> holdingRegs = {...};
     * std::vector<std::byte> packedBytes = packIntegerRegistersIntoBytes(holdingRegs);
     * @endcode
     */
    template<typename T>
    std::vector<std::byte> packIntegerRegistersIntoBytes(std::vector<T> &registers) {
        static_assert(std::is_same<T, HoldingRegister>::value || std::is_same<T, InputRegister>::value,
                      "packIntegerRegistersIntoBytes accepts only objects of type HoldingRegister "
                      "or InputRegister.");
        std::vector<std::byte> bytes;
        for (auto &reg: registers) {
            auto lsb = static_cast<std::byte>(reg.read() & 0xFF);
            auto msb = static_cast<std::byte>((reg.read() >> 8) & 0xFF);
            bytes.push_back(msb);
            bytes.push_back(lsb);
        }
        return bytes;
    }
} // namespace Modbus::Utilities

#endif //MBLIBRARY_MODBUSUTILITIES_H