#include <Modbus.h>
#include <ModbusDataArea.h>
#include <ModbusServer.h>


int main() {
    Modbus::DataArea dataArea;

    dataArea.generateCoils(0, 128, Modbus::ValueGenerationType::Random);
    dataArea.generateDiscreteInputs(0, 128, Modbus::ValueGenerationType::Random);
    dataArea.generateInputRegisters(0, 128, Modbus::ValueGenerationType::Random);
    dataArea.generateHoldingRegisters(0, 128, Modbus::ValueGenerationType::Random);

    Modbus::Server::Server server(dataArea);
    server.start();


    for (;;) {}

    return 0;
}