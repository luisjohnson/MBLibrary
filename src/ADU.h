//
// Created by luisjohnson on 3/30/2024.
//

#ifndef MBLIBRARY_ADU_H
#define MBLIBRARY_ADU_H

#include "ModbusPDU.h"

namespace Modbus {

    struct MBAP {
        uint16_t transactionIdentifier;
        uint16_t protocolIdentifier;
        uint16_t length;
        uint8_t unitIdentifier;
    };

    MBAP bytesToMBAP(const std::vector<std::byte> &bytes);

    std::vector<std::byte> MBAPToBytes(const MBAP &mbap);

    class ADU : public PDU {
    public:
        ADU(const std::vector<std::byte> &rawData, DataArea &modbusDataArea);

    private:
        MBAP _mbap;



    };

} // Modbus

#endif //MBLIBRARY_ADU_H
