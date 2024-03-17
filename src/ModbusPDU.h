//
// Created by Luis Johnson on 3/10/24.
//
#ifndef PDU_H
#define PDU_H

#include <cstddef>
#include <vector>
#include "Modbus.h"

/**
 * @class ModbusPDU
 * @brief Represents a Modbus Protocol Data Unit (ModbusPDU)
 *
 * The ModbusPDU class encapsulates the function code and data of a Modbus request or response.
 * It provides methods to construct and manipulate PDUs.
 */
class ModbusPDU {
public:
    ModbusPDU(ModbusFunctionCode functionCode, std::vector<std::byte> data);

    explicit ModbusPDU(ModbusFunctionCode functionCode);

    explicit ModbusPDU(std::vector<std::byte> rawData);

    ModbusFunctionCode functionCode() const;

    size_t size() const;

    std::vector<std::byte> getCompletePDU() const;

private:
    ModbusFunctionCode _functionCode;
    std::vector<std::byte> _data;
    size_t _completePduSize{};

    static bool isValidDataRaw(const std::vector<std::byte> &rawData);
};


#endif //PDU_H
