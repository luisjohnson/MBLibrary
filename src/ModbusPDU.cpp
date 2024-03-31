#include "Modbus.h"
#include "ModbusPDU.h"
#include "ModbusDataArea.h"

Modbus::MBAP Modbus::bytesToMBAP(const std::vector<std::byte> &bytes) {
    if (bytes.size() < 6)
        throw std::invalid_argument("Invalid number of bytes for MBAP.");
    MBAP mbap{};
    mbap.transactionIdentifier = (uint16_t) bytes[0] << 8 | (uint16_t) bytes[1];
    mbap.protocolIdentifier = (uint16_t) bytes[2] << 8 | (uint16_t) bytes[3];
    mbap.length = (uint16_t) bytes[4] << 8 | (uint16_t) bytes[5];
    mbap.unitIdentifier = (uint8_t) bytes[6];

    return mbap;
}

std::vector<std::byte> Modbus::MBAPToBytes(const MBAP &mbap) {
    std::vector<std::byte> bytes;
    bytes.push_back((std::byte) (mbap.transactionIdentifier >> 8));
    bytes.push_back((std::byte) (mbap.transactionIdentifier & 0xFF));
    bytes.push_back((std::byte) (mbap.protocolIdentifier >> 8));
    bytes.push_back((std::byte) (mbap.protocolIdentifier & 0xFF));
    bytes.push_back((std::byte) (mbap.length >> 8));
    bytes.push_back((std::byte) (mbap.length & 0xFF));
    return bytes;
}


Modbus::PDU::PDU(std::vector<std::byte> rawData, Modbus::DataArea &modbusDataArea)
        : _functionCode(Modbus::byteToModbusFunctionCode(rawData[0])), _modbusDataArea(modbusDataArea) {
    _data = std::vector<std::byte>(rawData.begin() + 1, rawData.end());
}

Modbus::PDU::PDU(Modbus::FunctionCode functionCode, std::vector<std::byte> data,
                 Modbus::DataArea &modbusDataArea) : _functionCode(functionCode),
                                                     _data(std::move(data)),
                                                     _modbusDataArea(modbusDataArea) {

}

Modbus::FunctionCode Modbus::PDU::getFunctionCode() {
    return _functionCode;
}

std::vector<std::byte> Modbus::PDU::buildResponse() {
    switch (_functionCode) {
        case Modbus::FunctionCode::ReadCoils:
            return getReadCoilsResponse();
        case Modbus::FunctionCode::ReadDiscreteInputs:
            return getReadDiscreteInputsResponse();
        case Modbus::FunctionCode::ReadHoldingRegisters:
            return getReadHoldingRegistersResponse();
        case Modbus::FunctionCode::ReadInputRegister:
            return getReadInputRegistersResponse();
        case Modbus::FunctionCode::WriteSingleCoil:
            return getWriteSingleCoilResponse();
        case Modbus::FunctionCode::WriteSingleRegister:
            return getWriteSingleRegisterResponse();
        case Modbus::FunctionCode::WriteMultipleCoils:
            return getWriteMultipleCoilsResponse();
        case Modbus::FunctionCode::WriteMultipleRegisters:
            return getWriteMultipleRegistersResponse();
        default:
            //  Build exception response for invalid function code
            return Modbus::buildExceptionResponse(_functionCode, Modbus::ExceptionCode::IllegalFunction);
    }
}

std::pair<uint16_t, uint16_t> Modbus::PDU::getStartingAddressAndQuantityOfRegisters() {
    auto startingAddress = Modbus::Utilities::twoBytesToUint16(_data[0], _data[1]);
    auto quantityOfRegisters = Modbus::Utilities::twoBytesToUint16(_data[2], _data[3]);
    return {startingAddress, quantityOfRegisters};
}


std::vector<std::byte> Modbus::PDU::getReadCoilsResponse() {
    // Get the starting address and quantity of coils
    auto [startingAddress, quantityOfCoils] = getStartingAddressAndQuantityOfRegisters();

    try {
        // Get the coils from the data area
        auto coils = _modbusDataArea.getCoils(startingAddress, quantityOfCoils);
        // Get the response
        return buildResponseForBooleanRegisters(coils, startingAddress, quantityOfCoils);
    } catch (std::out_of_range &e) {
        // Build response for invalid address
        return Modbus::buildExceptionResponse(_functionCode, Modbus::ExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> Modbus::PDU::getReadDiscreteInputsResponse() {
    auto [startingAddress, quantityOfRegisters] = getStartingAddressAndQuantityOfRegisters();

    try {
        // Get the discrete inputs from the data area
        auto discreteInputs = _modbusDataArea.getDiscreteInputs(startingAddress, quantityOfRegisters);
        // Get the response
        return buildResponseForBooleanRegisters(discreteInputs, startingAddress, quantityOfRegisters);
    } catch (std::out_of_range &e) {
        // Build response for invalid address
        return Modbus::buildExceptionResponse(_functionCode, Modbus::ExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> Modbus::PDU::getReadHoldingRegistersResponse() {
    auto [startingAddress, quantityOfRegisters] = getStartingAddressAndQuantityOfRegisters();

    try {
        // Get the holding registers from the data area
        auto holdingRegisters = _modbusDataArea.getHoldingRegisters(startingAddress, quantityOfRegisters);
        // Get the response
        return buildResponseForIntegerRegisters(holdingRegisters, startingAddress, quantityOfRegisters);

    } catch (std::out_of_range &e) {
        // Build response for invalid address
        return Modbus::buildExceptionResponse(_functionCode, Modbus::ExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> Modbus::PDU::getReadInputRegistersResponse() {
    auto [startingAddress, quantityOfRegisters] = getStartingAddressAndQuantityOfRegisters();

    try {
        // Get the input registers from the data area
        auto inputRegisters = _modbusDataArea.getInputRegisters(startingAddress, quantityOfRegisters);
        // Get the response
        return buildResponseForIntegerRegisters(inputRegisters, startingAddress, quantityOfRegisters);
    } catch (std::out_of_range &e) {
        // Build response for invalid address
        return Modbus::buildExceptionResponse(_functionCode, Modbus::ExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> Modbus::PDU::getWriteSingleCoilResponse() {
    int address = Modbus::Utilities::twoBytesToUint16(_data[1], _data[2]);
    int value = Modbus::Utilities::twoBytesToUint16(_data[3], _data[4]);
    bool coilValue = (value == 0xFF00);

    if (value != 0xFF00 && value != 0x0000) {
        return Modbus::buildExceptionResponse(_functionCode, Modbus::ExceptionCode::IllegalDataValue);
    }

    try {
        auto coil = _modbusDataArea.getCoils(address, 1).front();
        coil.write(coilValue);
        return {_data[0], _data[1], _data[2], _data[3], _data[4]};
    } catch (std::out_of_range &e) {
        return Modbus::buildExceptionResponse(_functionCode, Modbus::ExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> Modbus::PDU::getWriteSingleRegisterResponse() {
    auto [address, value] = getStartingAddressAndQuantityOfRegisters();
    try {
        auto holdingRegister = _modbusDataArea.getHoldingRegisters(address, 1).front();
        holdingRegister.write(value);
        return {_data[0], _data[1], _data[2], _data[3], _data[4]};
    } catch (std::out_of_range &e) {
        return Modbus::buildExceptionResponse(_functionCode, Modbus::ExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> Modbus::PDU::getWriteMultipleCoilsResponse() {
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
        auto coils = _modbusDataArea.getCoils(startingAddress, quantityOfCoils);
        for (int i = 0; i < quantityOfCoils; ++i) {
            coils[i].write(unpackedBooleans[i]);
        }

        return {_data[0], _data[1], _data[2], _data[3], _data[4]};

    } catch (std::out_of_range &e) {
        return Modbus::buildExceptionResponse(_functionCode, Modbus::ExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte> Modbus::PDU::getWriteMultipleRegistersResponse() {
    auto [startingAddress, quantityOfRegisters] = getStartingAddressAndQuantityOfRegisters();
    auto byteCount = static_cast<int>(_data[6]);
    try {
        auto holdingRegisters = _modbusDataArea.getHoldingRegisters(startingAddress, quantityOfRegisters);
        for (int i = 0; i < quantityOfRegisters; ++i) {
            auto value = Modbus::Utilities::twoBytesToUint16(_data[7 + i * 2], _data[8 + i * 2]);
            holdingRegisters[i].write(value);
        }
        return {_data[0], _data[1], _data[2], _data[3], _data[4]};
    } catch (std::out_of_range &e) {
        return Modbus::buildExceptionResponse(_functionCode, Modbus::ExceptionCode::IllegalDataAddress);
    }
}

std::vector<std::byte>Modbus::buildExceptionResponse(Modbus::FunctionCode functionCode, Modbus::ExceptionCode exceptionCode) {
    return {static_cast<std::byte>(0x80 + static_cast<uint8_t>(functionCode)),
            static_cast<std::byte>(exceptionCode)};
}
