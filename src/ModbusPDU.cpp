#include "ModbusUtilities.h"
#include "Modbus.h"
#include "ModbusPDU.h"


ModbusPDU::ModbusPDU(std::vector<std::byte> _rawData) : _functionCode(byteToModbusFunctionCode(_rawData[0])) {
    _data = std::vector<std::byte>(_rawData.begin() + 1, _rawData.end());
}

ModbusPDU::ModbusPDU(ModbusFunctionCode functionCode, std::vector<std::byte> data) : _functionCode(functionCode),
                                                                                     _data(std::move(data)) {
}

ModbusFunctionCode ModbusPDU::getFunctionCode() {
    return _functionCode;
}

//TODO Fix this
std::vector<std::byte> ModbusPDU::buildResponse(std::shared_ptr<ModbusDataArea> modbusDataArea) {
    switch (_functionCode) {
        case ModbusFunctionCode::ReadCoils:
            return getReadCoilsResponse(modbusDataArea);
        case ModbusFunctionCode::ReadDiscreteInputs:
            return {};
        default:
            //  Build exception response for invalid function code
            return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalFunction);
    }
}

//TODO Fix this
std::vector<std::byte> ModbusPDU::getReadCoilsResponse(ModbusDataArea modbusDataArea) {
    // Get the starting address and quantity of coils
    auto startingAddress = twoBytesToUint16(_data[0], _data[1]);
    auto quantityOfCoils = twoBytesToUint16(_data[3], _data[4]);

    try {
        // Get the coils from the data area
        auto coils = modbusDataArea.getCoils(startingAddress, quantityOfCoils);
        // Get the response
        return buildResponseForBooleanRegisters(coils, startingAddress, quantityOfCoils);
    } catch (std::out_of_range &e) {
        // Build response for invalid address
        return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> buildExceptionResponse(ModbusFunctionCode functionCode, ModbusExceptionCode exceptionCode) {
    return {static_cast<std::byte>(0x80 + static_cast<uint8_t>(functionCode)), static_cast<std::byte>(exceptionCode)};
}
