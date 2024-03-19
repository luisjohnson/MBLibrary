#include "ModbusPDU.h"

#include <utility>


ModbusPDU::ModbusPDU(std::vector<std::byte> _rawData){
    if(!isValidModbusFunctionCode(_rawData[0]))
        throw InvalidFunctionCodeException();
    _functionCode = static_cast<ModbusFunctionCode>(_rawData[0]);
    std::copy(_rawData.begin() + 1, _rawData.end(), _data.begin());
}

ModbusPDU::ModbusPDU(ModbusFunctionCode functionCode, std::vector<std::byte> _data) {

}
