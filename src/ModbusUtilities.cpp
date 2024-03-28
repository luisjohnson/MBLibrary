#include "ModbusUtilities.h"

namespace Modbus::Utilities {
    bool generateRandomBoolean() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution(0, 1);
        return distribution(gen);
    }

    int generateRandomInteger(int min, int max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution(min, max);
        return distribution(gen);
    }

    uint16_t twoBytesToUint16(std::byte msb, std::byte lsb) {
        return (static_cast<uint16_t>(msb) << 8) | static_cast<uint16_t>(lsb);
    }
}
