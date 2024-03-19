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
    explicit ModbusPDU(std::vector<std::byte> _rawData);

    ModbusPDU(ModbusFunctionCode functionCode, std::vector<std::byte> _data);

protected:
    ModbusFunctionCode _functionCode;
    std::vector<std::byte> _data;
};

#endif //PDU_H