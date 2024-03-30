#include "ModbusDataArea.h"
#include "ModbusUtilities.h"
#include "Modbus.h"
#include <utility>

Modbus::DataArea::DataArea() : _coils(), _discreteInputs(), _holdingRegisters(), _inputRegisters(), _mutex() {
}

void Modbus::DataArea::insertCoil(Modbus::Coil coil) {
    if (_coils.size() == Modbus::MAX_COILS)
        throw std::range_error("Maximum number of coils exceeded.");
    insertRegister(_coils, std::move(coil));
}

void Modbus::DataArea::insertDiscreteInput(Modbus::DiscreteInput input) {
    if (_discreteInputs.size() == Modbus::MAX_DISCRETE_INPUTS)
        throw std::range_error("Maximum number of discrete inputs exceeded.");
    insertRegister(_discreteInputs, std::move(input));
}

void Modbus::DataArea::insertHoldingRegister(Modbus::HoldingRegister holdingRegister) {
    if (_holdingRegisters.size() == Modbus::MAX_HOLDING_REGISTERS)
        throw std::range_error("Maximum number of holding registers exceeded.");
    insertRegister(_holdingRegisters, std::move(holdingRegister));
}

void Modbus::DataArea::insertInputRegister(Modbus::InputRegister inputRegister) {
    if (_inputRegisters.size() == Modbus::MAX_INPUT_REGISTERS)
        throw std::range_error("Maximum number of input registers exceeded.");
    insertRegister(_inputRegisters, std::move(inputRegister));
}

std::vector<Modbus::Coil> &Modbus::DataArea::getAllCoils() {
    return getAllRegisters(_coils);
}

std::vector<Modbus::DiscreteInput> &Modbus::DataArea::getAllDiscreteInputs() {
    return getAllRegisters(_discreteInputs);
}

std::vector<Modbus::HoldingRegister> &Modbus::DataArea::getAllHoldingRegisters() {
    return getAllRegisters(_holdingRegisters);
}

std::vector<Modbus::InputRegister> &Modbus::DataArea::getAllInputRegisters() {
    return getAllRegisters(_inputRegisters);
}

std::vector<Modbus::Coil> Modbus::DataArea::getCoils(int start, int length) {
    return getRegisters(_coils, start, length);
}

std::vector<Modbus::DiscreteInput> Modbus::DataArea::getDiscreteInputs(int start, int length) {
    return getRegisters(_discreteInputs, start, length);
}

std::vector<Modbus::HoldingRegister> Modbus::DataArea::getHoldingRegisters(int start, int length) {
    return getRegisters(_holdingRegisters, start, length);
}

std::vector<Modbus::InputRegister> Modbus::DataArea::getInputRegisters(int start, int length) {
    return getRegisters(_inputRegisters, start, length);
}

void Modbus::DataArea::generateCoils(int startAddress, int count, Modbus::ValueGenerationType type) {
    generateBooleanRegisters(_coils, startAddress, count, type);
}

void Modbus::DataArea::generateDiscreteInputs(int startAddress, int count, Modbus::ValueGenerationType type) {
    generateBooleanRegisters(_discreteInputs, startAddress, count, type);
}

void Modbus::DataArea::generateHoldingRegisters(int startAddress, int count, Modbus::ValueGenerationType type) {
    generateIntegerRegisters(_holdingRegisters, startAddress, count, type);
}

void Modbus::DataArea::generateInputRegisters(int startAddress, int count, Modbus::ValueGenerationType type) {
    generateIntegerRegisters(_inputRegisters, startAddress, count, type);
}
