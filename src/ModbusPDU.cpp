
#include <iterator>
#include "ModbusPDU.h"

ModbusPDU::ModbusPDU(const ModbusFunctionCode functionCode, std::vector<std::byte> data) : _functionCode(functionCode),
                                                                                           _data(std::move(data)),
                                                                                           _completePduSize(_data.size() + 1) {
}

ModbusPDU::ModbusPDU(const ModbusFunctionCode functionCode) : _functionCode(functionCode), _data({static_cast<std::byte>(0)}),
                                                              _completePduSize(2) {
}


ModbusPDU::ModbusPDU(std::vector<std::byte> rawData) {
    if(!isValidDataRaw(rawData))
        throw std::runtime_error("Incorrect data for ModbusPDU");
    _functionCode = static_cast<ModbusFunctionCode>(rawData[0]);
    std::ranges::copy(rawData.begin() + 1, rawData.end(), std::back_inserter(_data));
    _completePduSize = _data.size() + 1;
}

ModbusFunctionCode ModbusPDU::functionCode() const {
    return _functionCode;
}

size_t ModbusPDU::size() const {
    return _data.size() + 1;
}

std::vector<std::byte> ModbusPDU::getCompletePDU() const {
    std::vector<std::byte> transmitData;
    transmitData.push_back(static_cast<std::byte>(_functionCode));
    transmitData.insert(transmitData.end(), _data.begin(), _data.end());
    return transmitData;
}

bool ModbusPDU::isValidDataRaw(const std::vector<std::byte> &rawData) {
    return !rawData.empty() && isValidModbusFunctionCode(rawData[0]);
}


