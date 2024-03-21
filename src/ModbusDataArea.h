#include <memory>
#include <vector>
#include <mutex>
#include <algorithm>
#include "Modbus.h"

#ifndef MODBUSDATAAREA_H
#define MODBUSDATAAREA_H

class ModbusDataArea {
public:
    /**
     * @class ModbusDataArea
     * @brief Represents a data area for storing Modbus registers and coils.
     *
     * The ModbusDataArea class provides a container for storing Modbus registers and coils. It ensures
     * thread-safety by using a shared mutex to control access to the data. It also provides methods for
     * inserting, retrieving, and modifying the registers and coils.
     */
    ModbusDataArea();

    /**
     * @brief Inserts a ModbusCoil into the container.
     *
     * This function inserts a ModbusCoil object into the container.
     * The ModbusCoil object is inserted by reference via a shared_ptr.
     *
     * @param coil The ModbusCoil object to insert.
     */
    void insertCoil(ModbusCoil coil);


    /**
     * Inserts a discrete input into the Modbus device.
     *
     * @param input A shared pointer to a ModbusDiscreteInput object representing the input to be inserted.
     *
     * @see ModbusDiscreteInput
     */
    void insertDiscreteInput(ModbusDiscreteInput input);

    /**
     * @brief Inserts a ModbusHoldingRegister into the storage.
     *
     * This function is used to insert a ModbusHoldingRegister object into the storage, represented by a shared_ptr.
     *
     * @param holdingRegister A shared_ptr to the ModbusHoldingRegister object to be inserted.
     */
    void insertHoldingRegister(ModbusHoldingRegister holdingRegister);

    /**
     * @brief Inserts a ModbusInputRegister into the collection.
     *
     * This function inserts a ModbusInputRegister object into the collection.
     *
     * @param inputRegister The ModbusInputRegister object to be inserted.
     */
    void insertInputRegister(ModbusInputRegister inputRegister);

    /**
     * @defgroup Coils Coils
     * @brief Functions related to retrieving all coils
     */
    std::vector<ModbusCoil> &getAllCoils();

    /**
     * @brief Retrieve the state of all the discrete inputs.
     *
     * This function retrieves and returns the current state of all the discrete inputs in the system.
     *
     * @return A vector of boolean values representing the state of all the discrete inputs.
     */
    std::vector<ModbusDiscreteInput> &getAllDiscreteInputs();

    /**
     * @brief Retrieves all holding registers from a device.
     *
     * This function reads all holding registers from a device using a specific communication protocol and returns
     * the values in an array.
     *
     * @return An array containing all holding registers read from the device.
     */
    std::vector<ModbusHoldingRegister> &getAllHoldingRegisters();

    /**
     * @brief Returns an array of all input registers.
     *
     * This function retrieves and returns all the input registers from a device.
     *
     * @return An array of input registers.
     */
    std::vector<ModbusInputRegister> &getAllInputRegisters();

    /**
     * @brief Retrieves a range of coil values from a specific start index.
     *
     * This function retrieves a specified range of coil values from a given start index.
     *
     * @param start The starting index of the coil range to retrieve.
     * @param length The length of the coil range to retrieve.
     * @return The array of coil values from the specified range.
     * @note It is the responsibility of the caller to free the memory allocated for the returned array.
     * @note The start index should be a non-negative value, and the length should be a positive value.
     * @warning The function does not perform any boundary checks, and accessing out of range indices may lead to undefined behavior.
     */
    std::vector<ModbusCoil> getCoils(int start, int length);

    /**
     * @brief Gets the discrete inputs from a specific range
     *
     * This function retrieves the discrete inputs from a specified starting index with a specified length.
     * It returns a list of discrete input values.
     *
     * @param start The starting index of the range of discrete inputs to retrieve
     * @param length The length of the range of discrete inputs to retrieve
     *
     * @return A vector of bool values representing the discrete inputs
     */
    std::vector<ModbusDiscreteInput> getDiscreteInputs(int start, int length);

    /**
     * @brief Retrieves holding registers from a device.
     *
     * This function is responsible for fetching the holding registers data
     * from a device, starting from a specified address and retrieving a
     * specific number of registers.
     *
     * @param start The starting address of the holding registers.
     * @param length The number of holding registers to retrieve.
     * @return A vector of holding registers data.
     * @note Ensure that the device is properly connected before calling
     *       this function. Make sure that the start address and length values
     *       are valid for the device being used.
     */
    std::vector<ModbusHoldingRegister> getHoldingRegisters(int start, int length);

    /**
     * @brief Get the input registers from a Modbus device.
     *
     * This function retrieves a series of input registers from a Modbus device,
     * starting from a specified address and with a specified length.
     *
     * @param start The starting address of the input registers to retrieve.
     * @param length The number of input registers to retrieve.
     * @return A list of input registers retrieved from the Modbus device.
     */
    std::vector<ModbusInputRegister> getInputRegisters(int start, int length);

private:

    std::vector<ModbusCoil> _coils;
    std::vector<ModbusDiscreteInput> _discreteInputs;
    std::vector<ModbusHoldingRegister> _holdingRegisters;
    std::vector<ModbusInputRegister> _inputRegisters;
    std::mutex _mutex;

    /**
  * @brief Inserts a register into the given vector of registers and sorts the vector based on the register's address.
  *
  * This function inserts a register of type T into the provided vector of registers. It also ensures thread-safety by
  * acquiring a lock on the shared mutex before modifying the vector. If the size of the vector is greater than 1 after
  * the insertion, the vector is sorted in ascending order based on the register's address.
  *
  * @tparam T The type of register to insert.
  * @param registers The vector of registers to insert into.
  * @param reg The register to insert.
  *
  * Example usage:
  * ```cpp
  * std::vector<std::shared_ptr<Register>> registers;
  * auto register1 = std::make_shared<Register>(100);  // Suppose Register is a class and 100 is the address
  * auto register2 = std::make_shared<Register>(50);
  * insertRegister(registers, register1);
  * insertRegister(registers, register2);
  * ```
  * After running this code, the 'registers' vector will contain pointers to 'register2' and 'register1' in that order.
  */
    template<typename T>
    void insertRegister(std::vector<T> &registers, T reg) {
        std::lock_guard<std::mutex> lock(_mutex);
        registers.push_back(reg);
        if (registers.size() > 1)
            std::ranges::sort(registers, [](const auto &a, const auto &b) {
                return a.getAddress() < b.getAddress();
            });
    }

    /**
     * @brief Retrieves all registers from the provided vector of registers.
     *
     * This function returns a vector containing all registers from the provided vector of registers.
     * It ensures thread-safety by acquiring a lock on the shared mutex before accessing the vector.
     * The original vector is not modified.
     *
     * @tparam T The type of registers in the vector.
     * @param registers The vector of registers to retrieve.
     * @return std::vector<std::shared_ptr<T>> The vector containing all registers.
     */
    template<typename T>
    std::vector<T> &getAllRegisters(std::vector<T>& registers) {
        std::lock_guard<std::mutex> lock(_mutex);
        return registers;
    }

    /**
     * @brief Retrieves a range of registers from a given vector.
     *
     * This function allows to extract a range of registers from a provided vector
     * of shared pointers with a specified starting position and length. The registers
     * within the range are stored in the output vector, preserving their original order.
     *
     * @tparam T The type of the registers stored in the vector.
     * @param registers The vector containing shared pointers to the registers.
     * @param start The index indicating the starting position of the range.
     * @param length The length of the range of registers to retrieve.
     * @return None.
     *
     * @note If the starting position or length exceeds the size of the vector,
     *       the function retrieves as many registers as possible without throwing an exception.
     */
    template<typename T>
    std::vector<T> getRegisters(std::vector<T> &registers, int start, int length) {
        std::lock_guard<std::mutex> lock(_mutex);
        int end = start + length - 1;

        auto startIt = std::lower_bound(registers.begin(), registers.end(), start,
                                        [](const T &reg, int start) {
                                            return reg.getAddress() < start;
                                        });

        auto endIt = std::upper_bound(startIt, registers.end(), end,
                                      [](int end, const T &reg) {
                                          return end < reg.getAddress();
                                      });
        if (startIt == registers.end() || endIt == registers.begin())
            throw std::out_of_range("Requested range does not exist");

        return {startIt, endIt};
    }
};

#endif //MODBUSDATAAREA_H
