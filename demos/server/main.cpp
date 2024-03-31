#include <Modbus.h>
#include <ModbusDataArea.h>
#include <ModbusServer.h>


int main() {
    Modbus::DataArea dataArea;

    dataArea.generateCoils(0, 16, Modbus::ValueGenerationType::Random);
    dataArea.generateDiscreteInputs(0, 16, Modbus::ValueGenerationType::Random);
    dataArea.generateInputRegisters(0, 16, Modbus::ValueGenerationType::Random);
    dataArea.generateHoldingRegisters(0, 16, Modbus::ValueGenerationType::Random);

    Modbus::Server::MBServer server(dataArea);
    server.start();


    for (;;) {}

    return 0;
}