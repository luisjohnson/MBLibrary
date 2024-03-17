#include "ModbusDataArea.h"

ModbusDataArea::ModbusDataArea() : _coils(), _discreteInputs(), _holdingRegisters(), _inputRegisters(), _mutex() {
}

void ModbusDataArea::insertCoil(const std::shared_ptr<ModbusCoil> &coil) {
    insertRegister(_coils, coil);
}

void ModbusDataArea::insertDiscreteInput(const std::shared_ptr<ModbusDiscreteInput> &input) {
    insertRegister(_discreteInputs, input);
}

void ModbusDataArea::insertHoldingRegister(const std::shared_ptr<ModbusHoldingRegister> &holdingRegister) {
    insertRegister(_holdingRegisters, holdingRegister);
}

void ModbusDataArea::insertInputRegister(const std::shared_ptr<ModbusInputRegister> &inputRegister) {
    insertRegister(_inputRegisters, inputRegister);
}

std::vector<std::shared_ptr<ModbusCoil> > ModbusDataArea::getAllCoils() {
    return getAllRegisters(_coils);
}

std::vector<std::shared_ptr<ModbusDiscreteInput> > ModbusDataArea::getAllDiscreteInputs() {
    return getAllRegisters(_discreteInputs);
}

std::vector<std::shared_ptr<ModbusHoldingRegister> > ModbusDataArea::getAllHoldingRegisters() {
    return getAllRegisters(_holdingRegisters);
}

std::vector<std::shared_ptr<ModbusInputRegister> > ModbusDataArea::getAllInputRegisters() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _inputRegisters;
}

std::vector<std::shared_ptr<ModbusCoil> > ModbusDataArea::getCoils(int start, int length) {
    return getRegisters(_coils, start, length);
}

std::vector<std::shared_ptr<ModbusDiscreteInput> > ModbusDataArea::getDiscreteInputs(int start, int length) {
    return getRegisters(_discreteInputs, start, length);
}

std::vector<std::shared_ptr<ModbusHoldingRegister> > ModbusDataArea::getHoldingRegisters(int start, int length) {
    return getRegisters(_holdingRegisters, start, length);
}

std::vector<std::shared_ptr<ModbusInputRegister> > ModbusDataArea::getInputRegisters(int start, int length) {
    return getRegisters(_inputRegisters, start, length);
}