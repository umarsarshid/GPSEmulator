#pragma once
#include "ITransport.hpp"
#include <string>

class SerialPort : public ITransport {
public:
    SerialPort(const std::string& portName);
    ~SerialPort() override;


    bool connect() override;
    void disconnect();
    int send(const std::string& data) override;
private:
    std::string portName;
    int fileDescriptor; // The OS "ID card" for the open file
};
