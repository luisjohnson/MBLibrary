#include "ModbusDataArea.h"
#include "ModbusUtilities.h"
#include "Modbus.h"
#include <utility>

ModbusDataArea::ModbusDataArea() : _coils(), _discreteInputs(), _holdingRegisters(), _inputRegisters(), _mutex() {
}

void ModbusDataArea::insertCoil(Modbus::Coil coil) {
    if (_coils.size() == MAX_COILS)
        throw std::range_error("Maximum number of coils exceeded.");
    insertRegister(_coils, std::move(coil));
}

void ModbusDataArea::insertDiscreteInput(Modbus::DiscreteInput input) {
    if (_discreteInputs.size() == MAX_DISCRETE_INPUTS)
        throw std::range_error("Maximum number of discrete inputs exceeded.");
    insertRegister(_discreteInputs, std::move(input));
}

void ModbusDataArea::insertHoldingRegister(Modbus::HoldingRegister holdingRegister) {
    if (_holdingRegisters.size() == MAX_HOLDING_REGISTERS)
        throw std::range_error("Maximum number of holding registers exceeded.");
    insertRegister(_holdingRegisters, std::move(holdingRegister));
}

void ModbusDataArea::insertInputRegister(Modbus::InputRegister inputRegister) {
    if (_inputRegisters.size() == MAX_INPUT_REGISTERS)
        throw std::range_error("Maximum number of input registers exceeded.");
    insertRegister(_inputRegisters, std::move(inputRegister));
}

std::vector<Modbus::Coil> &ModbusDataArea::getAllCoils() {
    return getAllRegisters(_coils);
}

std::vector<Modbus::DiscreteInput> &ModbusDataArea::getAllDiscreteInputs() {
    return getAllRegisters(_discreteInputs);
}

std::vector<Modbus::HoldingRegister> &ModbusDataArea::getAllHoldingRegisters() {
    return getAllRegisters(_holdingRegisters);
}

std::vector<Modbus::InputRegister> &ModbusDataArea::getAllInputRegisters() {
    return getAllRegisters(_inputRegisters);
}

std::vector<Modbus::Coil> ModbusDataArea::getCoils(int start, int length) {
    return getRegisters(_coils, start, length);
}

std::vector<Modbus::DiscreteInput> ModbusDataArea::getDiscreteInputs(int start, int length) {
    return getRegisters(_discreteInputs, start, length);
}

std::vector<Modbus::HoldingRegister> ModbusDataArea::getHoldingRegisters(int start, int length) {
    return getRegisters(_holdingRegisters, start, length);
}

std::vector<Modbus::InputRegister> ModbusDataArea::getInputRegisters(int start, int length) {
    return getRegisters(_inputRegisters, start, length);
}

std::shared_ptr<ModbusDataArea> ModbusDataArea::getShared() {
    return shared_from_this();
}

void ModbusDataArea::generateCoils(int startAddress, int count, ValueGenerationType type) {
    generateBooleanRegisters(_coils, startAddress, count, type);
}


void ModbusDataArea::generateDiscreteInputs(int startAddress, int count, ValueGenerationType type) {
    generateBooleanRegisters(_discreteInputs, startAddress, count, type);
}

void ModbusDataArea::generateHoldingRegisters(int startAddress, int count, ValueGenerationType type) {
    generateIntegerRegisters(_holdingRegisters, startAddress, count, type);
}

void ModbusDataArea::generateInputRegisters(int startAddress, int count, ValueGenerationType type) {
    generateIntegerRegisters(_inputRegisters, startAddress, count, type);
}

