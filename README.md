# **Under Development**
 Modbus TCP Library

This Modbus TCP Library is designed with high performance in mind and uses modern C++ features.


## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

The Modbus TCP Library provides a robust and efficient implementation of the Modbus protocol for TCP/IP networks. It is designed to be easy to use and integrate into your projects, leveraging modern C++ features for optimal performance.

## Features

- High-performance Modbus TCP communication
- Modern C++ design
- Easy integration and usage
- Comprehensive test coverage

## Prerequisites

Before you begin, ensure you have met the following requirements:

- You have installed [Google Test](https://github.com/google/googletest).
- You have installed [Boost Libraries](https://www.boost.org/).


## Installation

To install the Modbus TCP Library, follow these steps:

1. Clone the repository:
    ```sh
    git clone <repository-url>
    ```

2. Navigate to the project directory:
    ```sh
    cd <repository-directory>
    ```

3. Build the project using CMake:
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

## Usage

To use the Modbus TCP Library in your project, include the necessary headers and link against the compiled library. Here is an example of how to include and use the library in your code:

```cpp
#include "Modbus.h"
#include "ModbusClient.h"
#include "ModbusServer.h"

// Example usage
int main() {
    ModbusClient client;
    ModbusServer server;

    // Initialize and start the client and server
    client.connect("127.0.0.1", 502);
    server.start(502);

    // Perform Modbus operations
    // ...

    return 0;
}
```

## Contributing 

Not looking for contributors at this time.

## License

This project is licensed under the MIT License. See the LICENSE file for more details. Modbus TCP Library

This Modbus TCP Library is designed with high performance in mind and uses modern C++ features.
