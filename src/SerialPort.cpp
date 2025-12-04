#include "SerialPort.hpp"
#include <fcntl.h> // Contains 'open' system call
#include <unistd.h> // Contains 'close' system call
#include <iostream>

SerialPort::SerialPort(const std::string& name) : portName(name), fileDescriptor(-1) {}

SerialPort::~SerialPort() { disconnect(); }

bool SerialPort::connect() {
    // O_RDWR = Read and Write
    // O_NOCTTY = This port shouldn't control my terminal (prevent CTRL-C weirdness)
    // O_NDELAY = Don't wait for the invisible wire to be "ready", just open it.
    fileDescriptor = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    
    if (fileDescriptor == -1) {
        perror("open_port: Unable to open /dev/ttyS0 - ");
        return false;
    }
    return true;
}

void SerialPort::disconnect() {
    if (fileDescriptor != -1) {
        close(fileDescriptor);
        fileDescriptor = -1;
    }
}
