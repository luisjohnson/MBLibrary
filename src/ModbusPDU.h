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
#include "ModbusUtilities.h"

namespace Modbus {

    /**
     * @file MBAP.h
     *
     * @brief This file contains the declaration of the MBAP struct.
     */
    struct MBAP {
        uint16_t transactionIdentifier;
        uint16_t protocolIdentifier;
        uint16_t length;
        uint8_t unitIdentifier;
    };


    /**
     * @brief Convert a vector of bytes to a MBAP (Modbus Application Protocol) structure.
     *
     * @param bytes The vector containing the bytes to be converted.
     * @return The converted MBAP structure.
     */
    MBAP bytesToMBAP(const std::vector<std::byte> &bytes);

    /**
     * @brief Converts a Modbus Application Protocol (MBAP) structure to a byte array
     *
     * This function converts a given MBAP structure to a byte array representation.
     *
     * @param mbap The MBAP structure to be converted
     * @return An array of bytes representing the MBAP structure
     *
     * @see MBAPFromBytes()
     */
    std::vector<std::byte> MBAPToBytes(const MBAP &mbap);


    /**
         * @class PDU
         * @brief Represents a Modbus Protocol Data Unit (PDU).
         *
         * This class encapsulates the raw data and provides methods to access and manipulate
         * the contents of the PDU. It is used in conjunction with DataArea to assist in
         * Modbus communication.
         */
    class PDU {
    public:
        /**
         * @class PDU
         * @brief Represents a Modbus Protocol Data Unit (PDU).
         *
         * This class encapsulates the raw data and provides methods to access and manipulate
         * the contents of the PDU. It is used in conjunction with DataArea to assist in
         * Modbus communication.
         */
        explicit PDU(std::vector<std::byte> rawData, Modbus::DataArea &modbusDataArea);

        PDU(Modbus::FunctionCode functionCode, std::vector<std::byte> data,
            Modbus::DataArea &modbusDataArea);


        /**
         * @brief Get the function code of the Modbus PDU.
         *
         * @return The Modbus function code.
         */
        FunctionCode getFunctionCode();


        /**
         * @brief Builds the response for the given request.
         *
         * This function analyzes the request and constructs an appropriate response based on the request data. The response is then returned as a string.
         *
         * @param request The request object containing the necessary information for building the response.
         * @return The constructed response as a string.
         */
        std::vector<std::byte> buildResponse();


    private:
        std::vector<std::byte> _data;
        FunctionCode _functionCode;
        DataArea &_modbusDataArea;

        /**
         * @brief Retrieves the starting address and quantity of registers.
         *
         * This function collects the starting address and quantity of registers
         * for a specific device. The details are retrieved from an external source
         * and returned as a tuple of integers.
         *
         * @return A tuple of two integers. The first integer represents the starting
         *         address of the registers, and the second integer represents the
         *         quantity of registers.
         *
         * @note The values for the starting address and quantity of registers are
         *       retrieved from an external source.
         *
         * @warning The external source must be properly initialized before calling
         *          this function to avoid unexpected behavior.
         *
         * @see initExternalSource()
         * @see deinitExternalSource()
         */
        std::pair<uint16_t, uint16_t> getStartingAddressAndQuantityOfRegisters();

        /**
         * @brief Returns the response for a Read Coils request.
         *
         * This function constructs and returns the response for a Read Coils request.
         *
         * @return The response as a vector of bytes.
         */
        std::vector<std::byte> getReadCoilsResponse();

        /**
         * @file
         * @brief Contains the declaration of the getReadDiscreteInputsResponse() function.
         */
        std::vector<std::byte> getReadDiscreteInputsResponse();

        /**
        *
        */
        std::vector<std::byte> getReadHoldingRegistersResponse();

        /**
         * @brief Function to get the response from reading input registers.
         *
         * This function retrieves the response from reading input register(s).
         *
         * @return ReadInputRegistersResponse - The response from reading input register(s).
         */
        std::vector<std::byte> getReadInputRegistersResponse();

        /**
         * @brief Returns the response for a Write Single Coil request.
         *
         * This function returns the response for a Write Single Coil request in Modbus protocol.
         * The response is represented as a byte array with certain format.
         *
         * @return The response byte array for Write Single Coil request.
         */
        std::vector<std::byte> getWriteSingleCoilResponse();

        /**
         * @brief Returns the response from writing a single register.
         *
         * This function returns the data received as a response after writing a single register.
         *
         * @return The response data received after writing a single register.
         */
        std::vector<std::byte> getWriteSingleRegisterResponse();

        /**
         * @brief Retrieves the response from the Modbus server after writing multiple coils.
         *
         * This function is used to get the response data from the Modbus server after sending a request to write multiple coils.
         * The response contains information about the success or failure of the write operation.
         *
         * @return The response data from the Modbus server for writing multiple coils.
         */
        std::vector<std::byte> getWriteMultipleCoilsResponse();

        /**
         * @brief Retrieves the response for "Write Multiple Registers" function code.
         *
         * This function retrieves the response received from a device after sending a "Write
         * Multiple Registers" request. The function reads the response and checks if it
         * was successful.
         *
         * @param[in] response The response received from the device.
         *
         * @return True if the response indicates a successful operation, false otherwise.
         */
        std::vector<std::byte> getWriteMultipleRegistersResponse();


        /**
         * @brief Builds the response for boolean registers.
         *
         * This function takes a vector of boolean registers, starting address, and quantity of registers as input,
         * and builds the response for the boolean registers based on the Modbus protocol.
         * The response is returned as a vector of bytes.
         *
         * @tparam T The type of the boolean registers.
         * @param booleanRegisters The vector of boolean registers.
         * @param startingAddress The starting address of the registers.
         * @param quantityOfRegisters The quantity of registers.
         * @return The response as a vector of bytes.
         *
         * @see packBooleanRegistersIntoBytes
         * @see calculateBytesFromBits
         */
        template<typename T>
        std::vector<std::byte>
        buildResponseForBooleanRegisters(std::vector<T> &booleanRegisters, int startingAddress,
                                         int quantityOfRegisters) {
            // Build the response
            auto byteCount = calculateBytesFromBits(booleanRegisters.size());
            auto functionCodeByte = static_cast<std::byte>(_functionCode);
            std::vector<std::byte> response(byteCount + 2);
            response[0] = functionCodeByte;
            response[1] = static_cast<std::byte>(byteCount);
            // Pack the coils into bytes
            auto packedBits = Utilities::packBooleanRegistersIntoBytes(booleanRegisters);
            // Copy the packed bits into the response
            std::copy(packedBits.begin(), packedBits.end(), response.begin() + 2);
            return response;
        }

        /**
         * @brief Builds the response for integer registers.
         *
         * This function takes a vector of integer registers, starting address, and quantity of registers
         * as input, and builds the response for the integer registers based on the Modbus protocol.
         * The response is returned as a vector of bytes.
         *
         * @tparam T The type of the integer registers.
         * @param integerRegisters The vector of integer registers.
         * @param startingAddress The starting address of the registers.
         * @param quantityOfRegisters The quantity of registers.
         * @return The response as a vector of bytes.
         *
         * @see packIntegerRegistersIntoBytes
         * @see calculateBytesFromIntegerRegisters
         */
        template<typename T>
        std::vector<std::byte>
        buildResponseForIntegerRegisters(std::vector<T> &integerRegisters, int startingAddress,
                                         int quantityOfRegisters) {
            //Build the response
            auto byteCount = quantityOfRegisters * 2;
            auto functionCodeByte = static_cast<std::byte>(_functionCode);
            std::vector<std::byte> response(byteCount + 2);
            response[0] = functionCodeByte;
            response[1] = static_cast<std::byte>(byteCount);
            std::vector<std::byte> packedBytes = Utilities::packIntegerRegistersIntoBytes(integerRegisters);
            std::copy(packedBytes.begin(), packedBytes.end(), response.begin() + 2);
            return response;
        }
    };

    /**
     * @brief Build an exception response for Modbus protocol.
     *
     * This function builds an exception response for the Modbus protocol. The exception
     * response is constructed as a vector of bytes, containing the function code and
     * the exception code.
     *
     * @param functionCode The function code representing the operation that caused the exception.
     * @param exceptionCode The exception code indicating the error condition.
     *
     * @return A vector of bytes representing the exception response.
     * @see Modbus::FunctionCode
     * @see Modbus::ExceptionCode
     * @code
     * auto response = Modbus::buildExceptionResponse(Modbus::FunctionCode::ReadCoils, Modbus::ExceptionCode::IllegalDataAddress);
     * @endcode
     */
    std::vector<std::byte>
    buildExceptionResponse(Modbus::FunctionCode functionCode, Modbus::ExceptionCode exceptionCode);
}


#endif //PDU_H
