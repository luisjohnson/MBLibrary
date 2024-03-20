#include "Modbus.h"
#include "ModbusPDU.h"

std::vector<std::byte> packBooleanRegistersIntoBytes(const std::vector<std::shared_ptr<ModbusCoil>>& vector1);

ModbusPDU::ModbusPDU(std::vector<std::byte> _rawData) {
    try {
        _functionCode = byteToModbusFunctionCode(_rawData[0]);
    } catch (InvalidFunctionCodeException &e) {
        // TODO Build response for invalid code
    }
}

ModbusPDU::ModbusPDU(ModbusFunctionCode functionCode, std::vector<std::byte> data) : _functionCode(functionCode),
                                                                                     _data(std::move(data)) {

}

ModbusFunctionCode ModbusPDU::getFunctionCode() {
    return _functionCode;
}

std::vector<std::byte> ModbusPDU::buildResponse(std::shared_ptr<ModbusDataArea> modbusDataArea) {
    switch (_functionCode) {
        case ModbusFunctionCode::ReadCoils:
            return getReadCoilsResponse(modbusDataArea);
        default:
            return {};
    }
}

std::vector<std::byte> ModbusPDU::getReadCoilsResponse(const std::shared_ptr<ModbusDataArea> &modbusDataArea) {
    auto startingAddressLsb = _data[0];
    auto startingAddressMsb = _data[1];
    auto startingAddress =
            (static_cast<uint16_t>(startingAddressMsb) << 8) | static_cast<uint16_t>(startingAddressLsb);
    auto quantityOfCoilsLsb = _data[3];
    auto quantityOfCoilsMsb = _data[4];
    auto quantityOfCoils =
            (static_cast<uint16_t>(quantityOfCoilsMsb) << 8 | static_cast<uint16_t>(quantityOfCoilsLsb));
    std::vector<std::shared_ptr<ModbusCoil>> coils = modbusDataArea->getCoils(startingAddress, quantityOfCoils);

    auto byteCount = calculateBytesFromBits(coils.size());

    std::vector<std::byte> response(byteCount + 2);
    auto functionCodeByte = static_cast<std::byte>(_functionCode);
    auto byteCountFirstByte = static_cast<std::byte>((byteCount >> 8) & 0xFF);
    auto byteCountSecondByte = static_cast<std::byte>(byteCount & 0xFF);

    response[0] = functionCodeByte;
    response[1] = byteCountFirstByte;
    response[2] = byteCountSecondByte;

    auto packedBits = packBooleanRegistersIntoBytes(coils);
    return response;

}

