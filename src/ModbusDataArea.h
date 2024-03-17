#include <memory>
#include <vector>
#include <mutex>
#include "Modbus.h"

#include "Modbus.h"

#ifndef MODBUSDATAAREA_H
#define MODBUSDATAAREA_H

class ModbusDataArea {
public:
    ModbusDataArea();

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
    void sortCoils();
    void sortDiscreteInputs();
    void sortHoldingRegisters();
    void sortInputRegisters();
    std::vector<std::shared_ptr<ModbusCoil> > _coils;
    std::vector<std::shared_ptr<ModbusDiscreteInput> > _discreteInputs;
    std::vector<std::shared_ptr<ModbusHoldingRegister> > _holdingRegisters;
    std::vector<std::shared_ptr<ModbusInputRegister> > _inputRegisters;
    std::mutex _mutex;
};

#endif //MODBUSDATAAREA_H
