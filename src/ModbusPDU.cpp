#include "ModbusUtilities.h"
#include "Modbus.h"
#include "ModbusPDU.h"

std::vector<std::byte> packBooleanRegistersIntoBytes(const std::vector<std::shared_ptr<ModbusCoil>> &vector1);

ModbusPDU::ModbusPDU(std::vector<std::byte> _rawData) : _functionCode(byteToModbusFunctionCode(_rawData[0])) {
    _data = std::vector<std::byte>(_rawData.begin() + 1, _rawData.end());
}

ModbusPDU::ModbusPDU(ModbusFunctionCode functionCode, std::vector<std::byte> data) : _functionCode(functionCode),
                                                                                     _data(std::move(data)) {

}

ModbusFunctionCode ModbusPDU::getFunctionCode() {
    return _functionCode;
}

std::vector<std::byte> ModbusPDU::buildResponse(const std::shared_ptr<ModbusDataArea>& modbusDataArea) {
    switch (_functionCode) {
        case ModbusFunctionCode::ReadCoils:
            return getReadCoilsResponse(modbusDataArea);
        default:
            return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalFunction);
    }
}

std::vector<std::byte> ModbusPDU::getReadCoilsResponse(const std::shared_ptr<ModbusDataArea> &modbusDataArea) {
    // Get the starting address and quantity of coils
    auto startingAddress = twoBytesToUint16(_data[0], _data[1]);
    auto quantityOfCoils = twoBytesToUint16(_data[3], _data[4]);

    try {
        // Get the coils from the data area
        auto coils = modbusDataArea->getCoils(startingAddress, quantityOfCoils);
        auto byteCount = calculateBytesFromBits(coils.size());

        // Build the response
        std::vector<std::byte> response(byteCount + 2);
        auto functionCodeByte = static_cast<std::byte>(_functionCode);
        auto byteCountFirstByte = static_cast<std::byte>((byteCount >> 8) & 0xFF);
        auto byteCountSecondByte = static_cast<std::byte>(byteCount & 0xFF);
        response[0] = functionCodeByte;
        response[1] = byteCountFirstByte;
        response[2] = byteCountSecondByte;
        // Pack the coils into bytes
        auto packedBits = packBooleanRegistersIntoBytes(coils);
        // Copy the packed bits into the response
        std::copy(packedBits.begin(), packedBits.end(), response.begin() + 3);
        return response;
    } catch (std::out_of_range &e) {
        // Build response for invalid address
        return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> buildExceptionResponse(ModbusFunctionCode functionCode, ModbusExceptionCode exceptionCode) {
    return {static_cast<std::byte>(0x80 + static_cast<uint8_t>(functionCode)), static_cast<std::byte>(exceptionCode)};
}
