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

    std::vector<bool> bytesToBooleans(const std::vector<std::byte> &bytes) {
    std::vector<bool> booleans(bytes.size() * 8);
    for (size_t i = 0; i < bytes.size(); ++i) {
        for (int j = 0; j < 8; ++j) {
            booleans[i * 8 + j] = (bytes[i] & static_cast<std::byte>(1 << j)) != std::byte(0);
        }
    }
    return booleans;
}

    std::pair<std::byte, std::byte> uint16ToTwoBytes(uint16_t value) {
        return {static_cast<std::byte>(value >> 8), static_cast<std::byte>(value & 0xFF)};
    }
}
