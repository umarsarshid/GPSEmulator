#pragma once
#include <string>

class SerialPort {
public:
    SerialPort(const std::string& portName);
    ~SerialPort();
    bool connect();
    void disconnect();
private:
    std::string portName;
    int fileDescriptor; // The OS "ID card" for the open file
};
