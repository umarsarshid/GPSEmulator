#include "SerialPort.hpp"
#include <fcntl.h> // Contains 'open' system call
#include <unistd.h> // Contains 'close' system call
#include <iostream>
#include <termios.h> 

SerialPort::SerialPort(const std::string& name) : portName(name), fileDescriptor(-1) {}

SerialPort::~SerialPort() { disconnect(); }
bool SerialPort::connect() {
    fileDescriptor = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fileDescriptor == -1) return false;

    // --- NEW CONFIGURATION LOGIC ---
    struct termios options;
    tcgetattr(fileDescriptor, &options); 

    // 1. Set Baud Rate (Standard 9600)
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    // 2. Set Raw Mode (Disable "smart" features like waiting for Enter)
    options.c_cflag |= (CLOCAL | CREAD); 
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); 

    tcsetattr(fileDescriptor, TCSANOW, &options);
    return true;
}

// Just a basic wrapper around write() for testing
int SerialPort::send(const std::string& data) {
    if (fileDescriptor == -1) return -1;
    return write(fileDescriptor, data.c_str(), data.size());
}


void SerialPort::disconnect() {
    if (fileDescriptor != -1) {
        close(fileDescriptor);
        fileDescriptor = -1;
    }
}
