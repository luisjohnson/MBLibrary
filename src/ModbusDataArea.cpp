#include "ModbusDataArea.h"

#include <utility>

ModbusDataArea::ModbusDataArea() : _coils(), _discreteInputs(), _holdingRegisters(), _inputRegisters(), _mutex() {
}

void ModbusDataArea::insertCoil(ModbusCoil coil) {
    insertRegister(_coils, std::move(coil));
}

void ModbusDataArea::insertDiscreteInput(ModbusDiscreteInput input) {
    insertRegister(_discreteInputs, std::move(input));
}

void ModbusDataArea::insertHoldingRegister(ModbusHoldingRegister holdingRegister) {
    insertRegister(_holdingRegisters, std::move(holdingRegister));
}

void ModbusDataArea::insertInputRegister(ModbusInputRegister inputRegister) {
    insertRegister(_inputRegisters, std::move(inputRegister));
}

std::vector<ModbusCoil> &ModbusDataArea::getAllCoils() {
    return getAllRegisters(_coils);
}

std::vector<ModbusDiscreteInput> &ModbusDataArea::getAllDiscreteInputs() {
    return getAllRegisters(_discreteInputs);
}

std::vector<ModbusHoldingRegister> &ModbusDataArea::getAllHoldingRegisters() {
    return getAllRegisters(_holdingRegisters);
}

std::vector<ModbusInputRegister> &ModbusDataArea::getAllInputRegisters() {
    return getAllRegisters(_inputRegisters);
}

std::vector<ModbusCoil> ModbusDataArea::getCoils(int start, int length) {
    return getRegisters(_coils, start, length);
}

std::vector<ModbusDiscreteInput> ModbusDataArea::getDiscreteInputs(int start, int length) {
    return getRegisters(_discreteInputs, start, length);
}

std::vector<ModbusHoldingRegister> ModbusDataArea::getHoldingRegisters(int start, int length) {
    return getRegisters(_holdingRegisters, start, length);
}

std::vector<ModbusInputRegister> ModbusDataArea::getInputRegisters(int start, int length) {
    return getRegisters(_inputRegisters, start, length);
}

std::shared_ptr<ModbusDataArea> ModbusDataArea::getShared() {
    return shared_from_this();
}
