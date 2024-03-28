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

/**
 * @class ModbusPDU
 * @brief Represents a Modbus Protocol Data Unit (PDU).
 *
 * This class encapsulates the raw data and provides methods to access and manipulate
 * the contents of the PDU. It is used in conjunction with ModbusDataArea to assist in
 * Modbus communication.
 */
class ModbusPDU {
public:
    /**
     * @class ModbusPDU
     * @brief Represents a Modbus Protocol Data Unit (PDU).
     *
     * This class encapsulates the raw data and provides methods to access and manipulate
     * the contents of the PDU. It is used in conjunction with ModbusDataArea to assist in
     * Modbus communication.
     */
    explicit ModbusPDU(std::vector<std::byte> rawData, std::shared_ptr<ModbusDataArea> modbusDataArea);

    ModbusPDU(ModbusFunctionCode functionCode, std::vector<std::byte> data,
              std::shared_ptr<ModbusDataArea> modbusDataArea);


    /**
     * @brief Get the function code of the Modbus PDU.
     *
     * @return The Modbus function code.
     */
    ModbusFunctionCode getFunctionCode();


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
    ModbusFunctionCode _functionCode;
    std::shared_ptr<ModbusDataArea> _modbusDataArea;

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
    buildResponseForBooleanRegisters(std::vector<T> &booleanRegisters, int startingAddress, int quantityOfRegisters) {
        // Build the response
        auto byteCount = calculateBytesFromBits(booleanRegisters.size());
        auto functionCodeByte = static_cast<std::byte>(_functionCode);
        std::vector<std::byte> response(byteCount + 2);
        response[0] = functionCodeByte;
        response[1] = static_cast<std::byte>(byteCount);
        // Pack the coils into bytes
        auto packedBits = packBooleanRegistersIntoBytes(booleanRegisters);
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
    buildResponseForIntegerRegisters(std::vector<T> &integerRegisters, int startingAddress, int quantityOfRegisters) {
        //Build the response
        auto byteCount = quantityOfRegisters*2;
        auto functionCodeByte = static_cast<std::byte>(_functionCode);
        std::vector<std::byte> response(byteCount + 2);
        response[0] = functionCodeByte;
        response[1] = static_cast<std::byte>(byteCount);
        std::vector<std::byte> packedBytes = packIntegerRegistersIntoBytes(integerRegisters);
        std::copy(packedBytes.begin(), packedBytes.end(), response.begin() + 2);
        return response;
    }
};


/**

   * \brief Builds an exception response for a given Modbus function code and exception code.
   *
   * This function generates an exception response based on the specified Modbus function code
   * and Modbus exception code. The generated response is in compliance with the Modbus Communication
   * Protocol specification.
   *
   * \param functionCode The Modbus function code for which the exception response is generated.
   * \param exceptionCode The Modbus exception code to be included in the exception response.
   *
   * \return The exception response packet as a byte array.
   *
   */
std::vector<std::byte> buildExceptionResponse(ModbusFunctionCode functionCode, ModbusExceptionCode exceptionCode);

#endif //PDU_H
