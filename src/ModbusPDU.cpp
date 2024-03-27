#include "Modbus.h"
#include "ModbusPDU.h"
#include "ModbusUtilities.h"


ModbusPDU::ModbusPDU(std::vector<std::byte> _rawData, std::shared_ptr<ModbusDataArea> modbusDataArea)
        : _functionCode(byteToModbusFunctionCode(_rawData[0])), _modbusDataArea(std::move(modbusDataArea)) {
    _data = std::vector<std::byte>(_rawData.begin() + 1, _rawData.end());
}

ModbusPDU::ModbusPDU(ModbusFunctionCode functionCode, std::vector<std::byte> data,
                     std::shared_ptr<ModbusDataArea> modbusDataArea) : _functionCode(functionCode),
                                                                       _data(std::move(data)),
                                                                       _modbusDataArea(std::move(modbusDataArea)) {

}

ModbusFunctionCode ModbusPDU::getFunctionCode() {
    return _functionCode;
}

std::vector<std::byte> ModbusPDU::buildResponse() {
    switch (_functionCode) {
        case ModbusFunctionCode::ReadCoils:
            return getReadCoilsResponse();
        case ModbusFunctionCode::ReadDiscreteInputs:
            return getReadDiscreteInputsResponse();
        case ModbusFunctionCode::ReadHoldingRegisters:
            return getReadHoldingRegistersResponse();
        case ModbusFunctionCode::ReadInputRegister:
            return getReadInputRegistersResponse();
        default:
            //  Build exception response for invalid function code
            return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalFunction);
    }
}

std::pair<uint16_t, uint16_t> ModbusPDU::getStartingAddressAndQuantityOfRegisters() {
    auto startingAddress = twoBytesToUint16(_data[0], _data[1]);
    auto quantityOfRegisters = twoBytesToUint16(_data[2], _data[3]);
    return {startingAddress, quantityOfRegisters};
}

std::vector<std::byte> buildExceptionResponse(ModbusFunctionCode functionCode, ModbusExceptionCode exceptionCode) {
    return {static_cast<std::byte>(0x80 + static_cast<uint8_t>(functionCode)), static_cast<std::byte>(exceptionCode)};
}


std::vector<std::byte> ModbusPDU::getReadCoilsResponse() {
    // Get the starting address and quantity of coils
    auto [startingAddress, quantityOfCoils] = getStartingAddressAndQuantityOfRegisters();

    try {
        // Get the coils from the data area
        auto coils = _modbusDataArea->getCoils(startingAddress, quantityOfCoils);
        // Get the response
        return buildResponseForBooleanRegisters(coils, startingAddress, quantityOfCoils);
    } catch (std::out_of_range &e) {
        // Build response for invalid address
        return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> ModbusPDU::getReadDiscreteInputsResponse() {
    auto [startingAddress, quantityOfRegisters] = getStartingAddressAndQuantityOfRegisters();

    try {
        // Get the discrete inputs from the data area
        auto discreteInputs = _modbusDataArea->getDiscreteInputs(startingAddress, quantityOfRegisters);
        // Get the response
        return buildResponseForBooleanRegisters(discreteInputs, startingAddress, quantityOfRegisters);
    } catch (std::out_of_range &e) {
        // Build response for invalid address
        return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> ModbusPDU::getReadHoldingRegistersResponse() {
    auto [startingAddress, quantityOfRegisters] = getStartingAddressAndQuantityOfRegisters();

    try {
        // Get the holding registers from the data area
        auto holdingRegisters = _modbusDataArea->getHoldingRegisters(startingAddress, quantityOfRegisters);
        // Get the response
        return buildResponseForIntegerRegisters(holdingRegisters, startingAddress, quantityOfRegisters);

    } catch (std::out_of_range &e) {
        // Build response for invalid address
        return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> ModbusPDU::getReadInputRegistersResponse() {
    auto [startingAddress, quantityOfRegisters] = getStartingAddressAndQuantityOfRegisters();

    try {
        // Get the input registers from the data area
        auto inputRegisters = _modbusDataArea->getInputRegisters(startingAddress, quantityOfRegisters);
        // Get the response
        return buildResponseForIntegerRegisters(inputRegisters, startingAddress, quantityOfRegisters);
    } catch (std::out_of_range &e) {
        // Build response for invalid address
        return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> ModbusPDU::getWriteSingleCoilResponse() {
    return {};
}

std::vector<std::byte> ModbusPDU::getWriteSingleRegisterResponse() {
    return {};
}

std::vector<std::byte> ModbusPDU::getWriteMultipleCoilsResponse() {
    return {};
}

std::vector<std::byte> ModbusPDU::getWriteMultipleRegistersResponse() {
    return {};
}
