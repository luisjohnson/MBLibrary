
#include "ModbusClient.h"

Modbus::Client::Client(const std::string &ip, const std::string &port) : _ip(ip), _port(port), _socket(_ioContext) {

}
