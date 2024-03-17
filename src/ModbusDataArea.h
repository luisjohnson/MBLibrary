#include <memory>
#include <vector>
#include <mutex>
#include <algorithm>
#include "Modbus.h"

#ifndef MODBUSDATAAREA_H
#define MODBUSDATAAREA_H

class ModbusDataArea {
public:
    ModbusDataArea();

    template<typename T>
    void insertRegister(std::vector<std::shared_ptr<T>> &registers, const std::shared_ptr<T> &reg) {
        std::lock_guard<std::mutex> lock(_mutex);
        registers.push_back(reg);
        if (registers.size() > 1)
            std::ranges::sort(registers, [](const auto &a, const auto &b) {
                return a->getAddress() < b->getAddress();
            });
    }

    template<typename T>
    std::vector<std::shared_ptr<T>> getAllRegisters(const std::vector<std::shared_ptr<T>> &registers) {
        std::lock_guard<std::mutex> lock(_mutex);
        return registers;
    }

    template<typename T>
    std::vector<std::shared_ptr<T>> getRegisters(std::vector<std::shared_ptr<T>> &registers, int start, int length) {
        std::lock_guard<std::mutex> lock(_mutex);
        int end = start + length - 1;

        auto startIt = std::lower_bound(registers.begin(), registers.end(), start,
                                        [](const auto &reg, int start) {
                                            return reg->getAddress() < start;
                                        });

        auto endIt = std::upper_bound(startIt, registers.end(), end,
                                      [](int end, const auto &reg) {
                                          return end < reg->getAddress();
                                      });
        if (startIt == registers.end() || endIt == registers.begin())
            throw std::out_of_range("Requested range does not exist");

        return {startIt, endIt};
    }


    void insertCoil(const std::shared_ptr<ModbusCoil> &coil);

    void insertDiscreteInput(const std::shared_ptr<ModbusDiscreteInput> &input);

    void insertHoldingRegister(const std::shared_ptr<ModbusHoldingRegister> &holdingRegister);

    void insertInputRegister(const std::shared_ptr<ModbusInputRegister> &inputRegister);

    std::vector<std::shared_ptr<ModbusCoil> > getAllCoils();

    std::vector<std::shared_ptr<ModbusDiscreteInput> > getAllDiscreteInputs();

    std::vector<std::shared_ptr<ModbusHoldingRegister> > getAllHoldingRegisters();

    std::vector<std::shared_ptr<ModbusInputRegister> > getAllInputRegisters();

    std::vector<std::shared_ptr<ModbusCoil> > getCoils(int start, int length);

    std::vector<std::shared_ptr<ModbusDiscreteInput> > getDiscreteInputs(int start, int length);

    std::vector<std::shared_ptr<ModbusHoldingRegister> > getHoldingRegisters(int start, int length);

    std::vector<std::shared_ptr<ModbusInputRegister> > getInputRegisters(int start, int length);

private:
    std::vector<std::shared_ptr<ModbusCoil> > _coils;
    std::vector<std::shared_ptr<ModbusDiscreteInput> > _discreteInputs;
    std::vector<std::shared_ptr<ModbusHoldingRegister> > _holdingRegisters;
    std::vector<std::shared_ptr<ModbusInputRegister> > _inputRegisters;
    std::mutex _mutex;
};

#endif //MODBUSDATAAREA_H
