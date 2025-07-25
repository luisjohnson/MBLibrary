#include <iostream>
#include <thread>
#include <Modbus.h>
#include <ModbusDataArea.h>
#include <ModbusServer.h>


/**
 * Clears the screen or display area, resetting it to a default state.
 */
void clearScreen()
{
#ifdef _WIN32
     std::system("cls");
#else
    std::system("clear");
#endif
}


int main()
{
    Modbus::DataArea dataArea;

    dataArea.generateCoils(0, 16, Modbus::ValueGenerationType::Random);
    dataArea.generateDiscreteInputs(0, 16, Modbus::ValueGenerationType::Random);
    dataArea.generateInputRegisters(0, 16, Modbus::ValueGenerationType::Random);
    dataArea.generateHoldingRegisters(0, 16, Modbus::ValueGenerationType::Random);

    std::thread printer([&dataArea]()
    {
        for (;;)
        {
            clearScreen();
            std::cout << std::left << std::setw(10) << "Address" << std::setw(10) << "Coils" << std::setw(20)
                << "Discrete Inputs" << std::setw(20)
                << "Holding Registers" << std::setw(20) << "Input Registers" << std::endl;
            for (int i = 0; i < 16; i++)
            {
                std::cout << std::setw(10) << i << std::setw(10) << (dataArea.getCoils(i, 1)[0].read() ? "1" : "0")
                    << std::setw(20)
                    << (dataArea.getDiscreteInputs(i, 1)[0].read() ? "1" : "0") << std::setw(20)
                    << dataArea.getHoldingRegisters(i, 1)[0].read() << std::setw(20)
                    << dataArea.getInputRegisters(i, 1)[0].read() << std::endl;
                //                std::cout << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    Modbus::Server::MBServer server(dataArea);
    server.start();

    printer.join();

    //    for (;;) {}
    return 0;
}
