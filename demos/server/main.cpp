#include <iostream>
#include <thread>
#include <Modbus.h>
#include <ModbusDataArea.h>
#include <ModbusServer.h>


int main() {
    Modbus::DataArea dataArea;

    dataArea.generateCoils(0, 16, Modbus::ValueGenerationType::Random);
    dataArea.generateDiscreteInputs(0, 16, Modbus::ValueGenerationType::Random);
    dataArea.generateInputRegisters(0, 16, Modbus::ValueGenerationType::Random);
    dataArea.generateHoldingRegisters(0, 16, Modbus::ValueGenerationType::Random);

    std::thread printer([&dataArea]() {

        for (;;) {
//            std::system("cls");
            std::cout << std::left << std::setw(10) << "Address" << std::setw(10) << "Coils" << std::setw(20)
                      << "Discrete Inputs" << std::setw(20)
                      << "Holding Registers" << std::setw(20) << "Input Registers" << std::endl;
            for (int i = 0; i < 16; i++) {
                std::cout << std::setw(10) << i << std::setw(10) << (dataArea.getCoils(i, 1)[0].read() ? "1" : "0")
                          << std::setw(20)
                          << (dataArea.getDiscreteInputs(i, 1)[0].read() ? "1" : "0") << std::setw(20)
                          << dataArea.getHoldingRegisters(i, 1)[0].read() << std::setw(20)
                          << dataArea.getInputRegisters(i, 1)[0].read() << std::endl;
//                std::cout << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
    });

    Modbus::Server::MBServer server(dataArea);
    server.start();

    printer.join();

//    for (;;) {}
    return 0;
}