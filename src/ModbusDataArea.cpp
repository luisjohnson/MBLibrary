#include <algorithm>
#include "ModbusDataArea.h"

ModbusDataArea::ModbusDataArea() : _coils(), _discreteInputs(), _holdingRegisters(), _inputRegisters(), _mutex() {
}

void ModbusDataArea::insertCoil(const std::shared_ptr<ModbusCoil> &coil) {
    std::lock_guard<std::mutex> lock(_mutex);
    _coils.push_back(coil);
    // DONE Troubleshoot sorting methods
    if (_coils.size() > 1)
        std::ranges::sort(_coils, [](const auto &a, const auto &b) {
            return a->getAddress() < b->getAddress();
        });
}

void ModbusDataArea::insertDiscreteInput(const std::shared_ptr<ModbusDiscreteInput> &input) {
    std::lock_guard<std::mutex> lock(_mutex);
    _discreteInputs.push_back(input);
    // DONE Troubleshoot sorting methods
    if (_discreteInputs.size() > 1)
        std::ranges::sort(_discreteInputs, [](const auto &a, const auto &b) {
            return a->getAddress() < b->getAddress();
        });
}

void ModbusDataArea::insertHoldingRegister(const std::shared_ptr<ModbusHoldingRegister> &holdingRegister) {
    std::lock_guard<std::mutex> lock(_mutex);
    _holdingRegisters.push_back(holdingRegister);
    // DONE Troubleshoot sorting methods
    if (_holdingRegisters.size() > 1)
        std::ranges::sort(_holdingRegisters, [](const auto &a, const auto &b) {
            return a->getAddress() < b->getAddress();
        });
}

void ModbusDataArea::insertInputRegister(const std::shared_ptr<ModbusInputRegister> &inputRegister) {
    std::lock_guard<std::mutex> lock(_mutex);
    // DONE Troubleshoot sorting methods
    _inputRegisters.push_back(inputRegister);
    if (_inputRegisters.size() > 1)
        std::ranges::sort(_inputRegisters, [](const auto &a, const auto &b) {
            return a->getAddress() < b->getAddress();
        });
}

std::vector<std::shared_ptr<ModbusCoil> > ModbusDataArea::getAllCoils() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _coils;
}

std::vector<std::shared_ptr<ModbusDiscreteInput> > ModbusDataArea::getAllDiscreteInputs() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _discreteInputs;
}

std::vector<std::shared_ptr<ModbusHoldingRegister> > ModbusDataArea::getAllHoldingRegisters() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _holdingRegisters;
}

std::vector<std::shared_ptr<ModbusInputRegister> > ModbusDataArea::getAllInputRegisters() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _inputRegisters;
}

std::vector<std::shared_ptr<ModbusCoil> > ModbusDataArea::getCoils(int start, int length) {
    int end = start + length - 1;
    std::lock_guard<std::mutex> lock(_mutex);

    auto startIt = std::lower_bound(_coils.begin(), _coils.end(), start,
                                    [](const auto &coil, int start) {
                                        return coil->getAddress() < start;
                                    });

    auto endIt = std::upper_bound(_coils.begin(), _coils.end(), end,
                                  [](int end, const auto &coil) {
                                      return end < coil->getAddress();
                                  });

    if (startIt == _coils.end() || endIt == _coils.begin()) {
        throw std::out_of_range("Requested range of coils does not exist");
    }
    return {startIt, endIt};

}

std::vector<std::shared_ptr<ModbusDiscreteInput> > ModbusDataArea::getDiscreteInputs(int start, int length) {
    int end = start + length - 1;
    std::lock_guard<std::mutex> lock(_mutex);

    auto startIt = std::lower_bound(_discreteInputs.begin(), _discreteInputs.end(), start,
                                    [](const auto &input, int start) {
                                        return input->getAddress() < start;
                                    });

    auto endIt = std::upper_bound(_discreteInputs.begin(), _discreteInputs.end(), end,
                                  [](int end, const auto &input) {
                                      return end < input->getAddress();
                                  });

    if (startIt == _discreteInputs.end() || endIt == _discreteInputs.begin()) {
        throw std::out_of_range("Requested range of discrete inputs does not exist");
    }
    return {startIt, endIt};
}

std::vector<std::shared_ptr<ModbusHoldingRegister> > ModbusDataArea::getHoldingRegisters(int start, int length) {
    int end = start + length - 1;
    std::lock_guard<std::mutex> lock(_mutex);

    auto startIt = std::lower_bound(_holdingRegisters.begin(), _holdingRegisters.end(), start,
                                    [](const auto &holdingRegister, int start) {
                                        return holdingRegister->getAddress() < start;
                                    });

    auto endIt = std::upper_bound(_holdingRegisters.begin(), _holdingRegisters.end(), end,
                                  [](int end, const auto &holdingRegister) {
                                      return end < holdingRegister->getAddress();
                                  });

    if (startIt == _holdingRegisters.end() || endIt == _holdingRegisters.begin()) {
        throw std::out_of_range("Requested range of holding registers does not exist");
    }
    return {startIt, endIt};
}

std::vector<std::shared_ptr<ModbusInputRegister> > ModbusDataArea::getInputRegisters(int start, int length) {
    int end = start + length - 1;
    std::lock_guard<std::mutex> lock(_mutex);

    auto startIt = std::lower_bound(_inputRegisters.begin(), _inputRegisters.end(), start,
                                    [](const auto &reg, int start) {
                                        return reg->getAddress() < start;
                                    });

    auto endIt = std::upper_bound(_inputRegisters.begin(), _inputRegisters.end(), end,
                                  [](int end, const auto &reg) {
                                      return end < reg->getAddress();
                                  });

    if (startIt == _inputRegisters.end() || endIt == _inputRegisters.begin()) {
        throw std::out_of_range("Requested range of input registers does not exist");
    }

    return {startIt, endIt};
}