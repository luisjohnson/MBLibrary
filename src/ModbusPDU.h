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
    ModbusPDU(std::vector<std::byte> _rawData);
    ModbusPDU(ModbusFunctionCode functionCode, std::vector<std::byte> data);
    ModbusFunctionCode getFunctionCode();
    std::vector<std::byte> buildResponse(const std::shared_ptr<ModbusDataArea>& modbusDataArea);


private:
    std::vector<std::byte> _data;
    ModbusFunctionCode _functionCode;
    std::vector<std::byte> getReadCoilsResponse(const std::shared_ptr<ModbusDataArea>& modbusDataArea);

};

std::vector<std::byte> buildExceptionResponse(ModbusFunctionCode functionCode, ModbusExceptionCode exceptionCode);

#endif //PDU_H