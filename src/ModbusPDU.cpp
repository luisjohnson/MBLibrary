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
        case ModbusFunctionCode::WriteSingleCoil:
            return getWriteSingleCoilResponse();
        case ModbusFunctionCode::WriteSingleRegister:
            return getWriteSingleRegisterResponse();
        case ModbusFunctionCode::WriteMultipleCoils:
            return getWriteMultipleCoilsResponse();
        case ModbusFunctionCode::WriteMultipleRegisters:
            return getWriteMultipleRegistersResponse();
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
    int address = twoBytesToUint16(_data[1], _data[2]);
    int value = twoBytesToUint16(_data[3], _data[4]);
    bool coilValue = (value == 0xFF00);

    if (value != 0xFF00 && value != 0x0000) {
        return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalDataValue);
    }

    try {
        auto coil = _modbusDataArea->getCoils(address, 1).front();
        coil.write(coilValue);
        return {_data[0], _data[1], _data[2], _data[3], _data[4]};
    } catch (std::out_of_range &e) {
        return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> ModbusPDU::getWriteSingleRegisterResponse() {
    auto [address, value] = getStartingAddressAndQuantityOfRegisters();
    try {
        auto holdingRegister = _modbusDataArea->getHoldingRegisters(address, 1).front();
        holdingRegister.write(value);
        return {_data[0], _data[1], _data[2], _data[3], _data[4]};
    } catch (std::out_of_range &e) {
        return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> ModbusPDU::getWriteMultipleCoilsResponse() {
    auto [startingAddress, quantityOfCoils] = getStartingAddressAndQuantityOfRegisters();
    auto byteCount = static_cast<int>(_data[6]);

    std::vector<bool> unpackedBooleans{};

    for (int i = 0; i < byteCount; ++i) {
        auto byte = static_cast<uint8_t>(_data[7 + i]);
        for (int j = 0; j < 8; ++j) {
            unpackedBooleans.push_back((byte & (1 << j)) != 0);
        }
    }

    try {
        auto coils = _modbusDataArea->getCoils(startingAddress, quantityOfCoils);
        for (int i = 0; i < quantityOfCoils; ++i) {
            coils[i].write(unpackedBooleans[i]);
        }

        return {_data[0], _data[1], _data[2], _data[3], _data[4]};

    } catch (std::out_of_range &e) {
        return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> ModbusPDU::getWriteMultipleRegistersResponse() {
    auto [startingAddress, quantityOfRegisters] = getStartingAddressAndQuantityOfRegisters();
    auto byteCount = static_cast<int>(_data[6]);
    try {
        auto holdingRegisters = _modbusDataArea->getHoldingRegisters(startingAddress, quantityOfRegisters);
        for (int i = 0; i < quantityOfRegisters; ++i) {
            auto value = twoBytesToUint16(_data[7 + i * 2], _data[8 + i * 2]);
            holdingRegisters[i].write(value);
        }
        return {_data[0], _data[1], _data[2], _data[3], _data[4]};
    } catch (std::out_of_range &e) {
        return buildExceptionResponse(_functionCode, ModbusExceptionCode::IllegalDataAddress);
    }
}
