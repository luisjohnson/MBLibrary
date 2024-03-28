#include <memory>
#include <vector>
#include <mutex>
#include <algorithm>
#include "Modbus.h"
#include "ModbusUtilities.h"

#ifndef MODBUSDATAAREA_H
#define MODBUSDATAAREA_H

namespace Modbus {
    constexpr int MAX_COILS = 2000;
    constexpr int MAX_DISCRETE_INPUTS = 2000;
    constexpr int MAX_HOLDING_REGISTERS = 125;
    constexpr int MAX_INPUT_REGISTERS = 125;
    /**
         * @enum ValueGenerationType
         * @brief Enumeration for different types of value generation.
         *
         * This enumeration defines the different types of value generation that can be used.
         * The available types are:
         *   - Random: Generate random values.
         *   - Incremental: Generate values in incremental order.
         *   - Decremental: Generate values in decremental order.
         *   - Zeros: Generate values as zeros.
         *   - Ones: Generate values as ones.
         */
    enum class ValueGenerationType {
        Random,
        Incremental,
        Decremental,
        Zeros,
        Ones
    };

    /**
     * @class DataArea
     * @brief Represents a data area for storing Modbus registers and coils.
     *
     * The DataArea class provides a container for storing Modbus registers and coils. It ensures
     * thread-safety by using a shared mutex to control access to the data. It also provides methods for
     * inserting, retrieving, and modifying the registers and coils.
     */
    class DataArea : public std::enable_shared_from_this<DataArea> {
    public:
        /**
         * @class DataArea
         * @brief Represents a data area for storing Modbus registers and coils.
         *
         * The DataArea class provides a container for storing Modbus registers and coils. It ensures
         * thread-safety by using a shared mutex to control access to the data. It also provides methods for
         * inserting, retrieving, and modifying the registers and coils.
         */
        DataArea();

        /**
         * @brief Inserts a Coil into the container.
         *
         * This function inserts a Coil object into the container.
         * The Coil object is inserted by reference via a shared_ptr.
         *
         * @param coil The Coil object to insert.
         */
        void insertCoil(Coil coil);


        /**
         * Inserts a discrete input into the Modbus device.
         *
         * @param input A shared pointer to a DiscreteInput object representing the input to be inserted.
         *
         * @see DiscreteInput
         */
        void insertDiscreteInput(DiscreteInput input);

        /**
         * @brief Inserts a HoldingRegister into the storage.
         *
         * This function is used to insert a HoldingRegister object into the storage, represented by a shared_ptr.
         *
         * @param holdingRegister A shared_ptr to the HoldingRegister object to be inserted.
         */
        void insertHoldingRegister(HoldingRegister holdingRegister);

        /**
         * @brief Inserts a InputRegister into the data area.
         *
         * This function is used to insert a InputRegister into the data area.
         * If the maximum number of input registers has been reached, a std::range_error
         * exception will be thrown.
         *
         * @param inputRegister The InputRegister to be inserted.
         *
         * @throws std::range_error Thrown if the maximum number of input registers has been exceeded.
         */
        void insertInputRegister(InputRegister inputRegister);

        /**
         * @brief Generates coils in the Modbus data area.
         *
         * This function generates coils in the Modbus data area starting from the specified address.
         * The number of coils to be generated is determined by the count parameter. By default, the
         * coils are generated using the ValueGenerationType::Zeros option, which sets all the coils to 0.
         *
         * @param startAddress The starting address for generating the coils.
         * @param count The number of coils to be generated.
         * @param type The type of value generation to be used, defaults to ValueGenerationType::Zeros.
         *
         * @see DataArea::generateBooleanRegisters()
         */
        void generateCoils(int startAddress, int count, ValueGenerationType type = ValueGenerationType::Zeros);

        /**
         * @brief Generates discrete inputs in a Modbus data area.
         *
         * This function generates a specified number of discrete inputs in a Modbus data area,
         * starting from a specified address. The generated values are based on the specified value generation type.
         *
         * @param startAddress The start address of the discrete inputs.
         * @param count The number of discrete inputs to generate.
         * @param type The value generation type. Default is ValueGenerationType::Zeros.
         *
         * @details The generated discrete inputs are stored in the '_discreteInputs' member variable of the DataArea class.
         * The generated values are based on the specified value generation type, which can be one of the following:
         * - ValueGenerationType::Zeros: All generated values are set to zero.
         * - ValueGenerationType::Ones: All generated values are set to one.
         * - ValueGenerationType::Random: All generated values are randomly set to zero or one.
         *
         * @see DataArea::generateBooleanRegisters
         */
        void generateDiscreteInputs(int startAddress, int count, ValueGenerationType type = ValueGenerationType::Zeros);

        /**
         * @brief Generate holding registers with specified starting address, count, and value generation type.
         *
         * This function generates holding registers in the specified Modbus data area starting from the given
         * start address. The number of registers to generate is determined by the count parameter. The value
         * generation type can be optionally specified. By default, the generated registers will be filled with zeros.
         *
         * @param startAddress The starting address of the holding registers to be generated.
         * @param count The number of holding registers to generate.
         * @param type The value generation type (optional). Defaults to ValueGenerationType::Zeros.
         *
         * @sa DataArea::generateHoldingRegisters
         */
        void
        generateHoldingRegisters(int startAddress, int count, ValueGenerationType type = ValueGenerationType::Zeros);

        /**
         * @brief Generates input registers for Modbus data area.
         *
         * This function generates input registers with specified start address, count, and value generation type.
         * The generated input registers will be stored in the _inputRegisters array of the Modbus data area.
         *
         * @param startAddress The starting address of the input registers.
         * @param count The number of input registers to generate.
         * @param type The value generation type. It has a default value of ValueGenerationType::Zeros.
         *             Possible value generation types are:
         *             - ValueGenerationType::Zeros: Generates input registers with all zeros.
         *             - ValueGenerationType::Random: Generates input registers with random values.
         *             - ValueGenerationType::Incremental: Generates input registers with incremental values starting from the startAddress.
         */
        void generateInputRegisters(int startAddress, int count, ValueGenerationType type = ValueGenerationType::Zeros);

        /**
         * @defgroup Coils Coils
         * @brief Functions related to retrieving all coils
         */
        std::vector<Coil> &getAllCoils();

        /**
         * @brief Retrieve the state of all the discrete inputs.
         *
         * This function retrieves and returns the current state of all the discrete inputs in the system.
         *
         * @return A vector of boolean values representing the state of all the discrete inputs.
         */
        std::vector<DiscreteInput> &getAllDiscreteInputs();

        /**
         * @brief Retrieves all holding registers from a device.
         *
         * This function reads all holding registers from a device using a specific communication protocol and returns
         * the values in an array.
         *
         * @return An array containing all holding registers read from the device.
         */
        std::vector<HoldingRegister> &getAllHoldingRegisters();

        /**
         * @brief Returns an array of all input registers.
         *
         * This function retrieves and returns all the input registers from a device.
         *
         * @return An array of input registers.
         */
        std::vector<InputRegister> &getAllInputRegisters();

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
        std::vector<Coil> getCoils(int start, int length);

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
        std::vector<DiscreteInput> getDiscreteInputs(int start, int length);

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
        std::vector<HoldingRegister> getHoldingRegisters(int start, int length);

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
        std::vector<InputRegister> getInputRegisters(int start, int length);

        /**
         * @brief Retrieves a shared pointer to the DataArea object.
         *
         * This function returns a shared pointer to the DataArea object.
         * The DataArea class represents a data area for storing Modbus registers and coils. It ensures thread-safety
         * by using a shared mutex to control access to the data. Clients can use this function to obtain a shared pointer
         * to the DataArea object and safely access its methods and members.
         *
         *
         * @return std::shared_ptr<DataArea> A shared pointer to the DataArea object.
         */
        std::shared_ptr<DataArea> getShared();


    private:

        std::vector<Coil> _coils;
        std::vector<DiscreteInput> _discreteInputs;
        std::vector<HoldingRegister> _holdingRegisters;
        std::vector<InputRegister> _inputRegisters;
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
        std::vector<T> &getAllRegisters(std::vector<T> &registers) {
            std::lock_guard<std::mutex> lock(_mutex);
            return registers;
        }


        /**
         * @brief Retrieves a portion of registers from a vector.
         *
         * This function retrieves a specified range of registers from a given vector. It modifies the `registers`
         * vector and fills it with the requested registers.
         *
         * @tparam T The data type of the registers.
         * @param registers The vector to retrieve the registers from.
         * @param start The starting index of the registers to retrieve.
         * @param length The number of registers to retrieve.
         *
         * @note The function assumes that the `registers` vector has at least `start + length` elements to avoid
         * any out-of-bounds access. If the `registers` vector has fewer elements, the behavior is undefined.
         *
         * @return void
         */
        template<typename T>
        std::vector<T> getRegisters(std::vector<T> &registers, int start, int length) {
            std::lock_guard<std::mutex> lock(_mutex);
            // Calculate the end index of the range
            int end = start + length - 1;
            // Throwing an exception if the requested range is invalid
            if (length > 2000 || length > registers.size())
                throw std::out_of_range("Requested range is invalid");
            // Find the start and end iterators for the requested range
            auto startIt = std::lower_bound(registers.begin(), registers.end(), start,
                                            [](const T &reg, int start) {
                                                return reg.getAddress() < start;
                                            });
            auto endIt = std::upper_bound(startIt, registers.end(), end,
                                          [](int end, const T &reg) {
                                              return end < reg.getAddress();
                                          });
            // Throwing an exception if the requested range does not exist
            if (startIt == registers.end() || endIt == registers.begin())
                throw std::out_of_range("Requested range does not exist");

            return {startIt, endIt};
        }


        /**
         * @brief Generates a series of boolean registers of type `Coil` or `DiscreteInput` based on the specified value generation type.
         *
         * @tparam T The type of register (Coil or DiscreteInput).
         * @param registers The vector of registers to store the generated registers.
         * @param startAddress The starting address for the generated registers.
         * @param count The number of registers to generate.
         * @param type The value generation type.
         *
         * @throws std::invalid_argument if an invalid value generation type is provided.
         *
         * This function generates boolean registers of the specified type (`Coil` or `DiscreteInput`) and inserts them into the provided vector `registers`.
         * The generation of the registers depends on the specified value generation type. Supported value generation types are Zeros, Ones, and Random.
         *
         * If the value generation type is Zeros, the function generates registers with a value of `false` and inserts them into the vector `registers`.
         * If the value generation type is Ones, the function generates registers with a value of `true` and inserts them into the vector `registers`.
         * If the value generation type is Random, the function generates registers with a random boolean value and inserts them into the vector `registers`.
         *
         * @note The function is thread-safe and uses a mutex to synchronize access to the vector `registers`.
         *
         * Example usage:
         *
         * ```cpp
         * std::vector<Coil> registers;
         * generateBooleanRegisters(registers, 1000, 10, ValueGenerationType::Random);
         * ```
         */
        template<typename T>
        void generateBooleanRegisters(std::vector<T> &registers, int startAddress, int count,
                                      ValueGenerationType type) {
            static_assert(std::is_same<T, Coil>::value || std::is_same<T, DiscreteInput>::value,
                          "Invalid register type, register type must be Coil or DiscreteInput.");
            switch (type) {
                case ValueGenerationType::Zeros:
                    for (int i = 0; i < count; i++) {
                        insertRegister(registers, T(startAddress + i, false));
                    }
                    break;
                case ValueGenerationType::Ones:
                    for (int i = 0; i < count; i++) {
                        insertRegister(registers, T(startAddress + i, true));
                    }
                    break;
                case ValueGenerationType::Random:
                    for (int i = 0; i < count; i++) {
                        insertRegister(registers, T(startAddress + i, Utilities::generateRandomBoolean()));
                    }
                    break;
                case ValueGenerationType::Decremental:
                case ValueGenerationType::Incremental:
                default:
                    throw std::invalid_argument("Invalid value generation type.");
            }
        }


        /**
         * \brief Generates integer registers based on the given parameters.
         *
         * \tparam T - The type of the registers.
         * \param registers - The vector of registers to generate.
         * \param startAddress - The starting address of the registers.
         * \param count - The number of registers to generate.
         * \param type - The value generation type.
         *
         * This function generates integer registers based on the given parameters.
         * The registers are inserted into the provided vector.
         *
         * Possible value generation types are:
         * - Zeros: generates registers with value 0.
         * - Ones: generates registers with value 1.
         * - Random: generates registers with random integer values.
         * - Decremental: generates registers with values in decremental order (from count to 1).
         * - Incremental: generates registers with values in incremental order (from 0 to count-1).
         *
         * \throws std::invalid_argument if an invalid value generation type is provided.
         * \throws std::invalid_argument if an invalid register type is provided (must be HoldingRegister or InputRegister).
         */
        template<typename T>
        void generateIntegerRegisters(std::vector<T> &registers, int startAddress, int count,
                                      ValueGenerationType type) {
            static_assert(std::is_same<T, HoldingRegister>::value || std::is_same<T, InputRegister>::value,
                          "Invalid register type, register type must be HoldingRegister or InputRegister.");
            switch (type) {
                case ValueGenerationType::Zeros:
                    for (int i = 0; i < count; i++) {
                        insertRegister(registers, T(startAddress + i, 0));
                    }
                    break;
                case ValueGenerationType::Ones:
                    for (int i = 0; i < count; i++) {
                        insertRegister(registers, T(startAddress + i, 1));
                    }
                    break;
                case ValueGenerationType::Random:
                    for (int i = 0; i < count; i++) {
                        insertRegister(registers, T(startAddress + i, Utilities::generateRandomInteger()));
                    }
                    break;
                case ValueGenerationType::Decremental:
                    for (int i = 0; i < count; i++) {
                        insertRegister(registers, T(startAddress + i, count - i));
                    }
                    break;
                case ValueGenerationType::Incremental:
                    for (int i = 0; i < count; i++) {
                        insertRegister(registers, T(startAddress + i, i));
                    }
                    break;
                default:
                    throw std::invalid_argument("Invalid value generation type.");
            }

        }
    };
}
#endif //MODBUSDATAAREA_H
