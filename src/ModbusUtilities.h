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

    uint16_t twoBytesToUint16(std::byte msb, std::byte lsb);

    bool generateRandomBoolean();

    int generateRandomInteger(int min = INT_MIN, int max = INT_MAX);

    template<typename T>
    std::vector<std::byte> packBooleanRegistersIntoBytes(std::vector<T> &registers);

    template<typename T>
    std::vector<std::byte> packIntegerRegistersIntoBytes(std::vector<T> &registers);


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

    template<typename T>
    std::vector<std::byte> packIntegerRegistersIntoBytes(std::vector<T> &registers) {
        static_assert(std::is_same<T, ModbusHoldingRegister>::value || std::is_same<T, ModbusInputRegister>::value,
                      "packIntegerRegistersIntoBytes accepts only objects of type ModbusHoldingRegister "
                      "or ModbusInputRegister.");
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