//
// Created by luisjohnson on 3/30/2024.
//

#include "ADU.h"

namespace Modbus {
    MBAP bytesToMBAP(const std::vector<std::byte> &bytes) {
        if (bytes.size() < 6)
            throw std::invalid_argument("Invalid number of bytes for MBAP.");
        MBAP mbap;
        mbap.transactionIdentifier = (uint16_t)bytes[0] << 8 | (uint16_t)bytes[1];
        mbap.protocolIdentifier = (uint16_t)bytes[2] << 8 | (uint16_t)bytes[3];
        mbap.length = (uint16_t)bytes[4] << 8 | (uint16_t)bytes[5];
        mbap.unitIdentifier = (uint8_t)bytes[6];

        return mbap;
    }

    std::vector<std::byte> MBAPToBytes(const MBAP &mbap) {
        std::vector<std::byte> bytes;
        bytes.push_back((std::byte)(mbap.transactionIdentifier >> 8));
        bytes.push_back((std::byte)(mbap.transactionIdentifier & 0xFF));
        bytes.push_back((std::byte)(mbap.protocolIdentifier >> 8));
        bytes.push_back((std::byte)(mbap.protocolIdentifier & 0xFF));
        bytes.push_back((std::byte)(mbap.length >> 8));
        bytes.push_back((std::byte)(mbap.length & 0xFF));
        return bytes;
    }

    ADU::ADU(const std::vector<std::byte> &rawData, DataArea &modbusDataArea) : PDU(rawData, modbusDataArea) {}
} // Modbus