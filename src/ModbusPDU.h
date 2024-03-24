// Version: 1.0
// Author: Luis Johnson
// Date: 3/10/2024
//
#ifndef PDU_H
#define PDU_H

#include <cstddef>
#include <vector>
#include <memory>
#include "Modbus.h"
#include "ModbusDataArea.h"

class ModbusPDU {
public:
    explicit ModbusPDU(std::vector<std::byte> _rawData, std::shared_ptr<ModbusDataArea> modbusDataArea);

    ModbusPDU(ModbusFunctionCode functionCode, std::vector<std::byte> data,
              std::shared_ptr<ModbusDataArea> modbusDataArea);

    ModbusFunctionCode getFunctionCode();

    std::vector<std::byte> buildResponse();

private:
    std::vector<std::byte> _data;
    ModbusFunctionCode _functionCode;
    std::shared_ptr<ModbusDataArea> _modbusDataArea;

    std::vector<std::byte> getReadCoilsResponse();

    template<typename T>
    std::vector<std::byte>
    buildResponseForBooleanRegisters(std::vector<T> &booleanRegisters, int startingAddress, int quantityOfRegisters) {
        // Build the response
        auto byteCount = calculateBytesFromBits(booleanRegisters.size());
        auto functionCodeByte = static_cast<std::byte>(_functionCode);
        std::vector<std::byte> response(byteCount + 3);
        response[0] = functionCodeByte;
        response[1] = static_cast<std::byte>(byteCount);
        // Pack the coils into bytes
        auto packedBits = packBooleanRegistersIntoBytes(booleanRegisters);
        // Copy the packed bits into the response
        std::copy(packedBits.begin(), packedBits.end(), response.begin() + 2);
        return response;
    }
};

std::vector<std::byte> buildExceptionResponse(ModbusFunctionCode functionCode, ModbusExceptionCode exceptionCode);

#endif //PDU_H
