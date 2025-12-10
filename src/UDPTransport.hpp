#pragma once
#include "ITransport.hpp"
#include <string>
#include <netinet/in.h>

class UDPTransport : public ITransport {
public:
    UDPTransport(const std::string& ip, int port);
    ~UDPTransport() override;
    bool connect() override;
    int send(const std::string& data) override;

private:
    std::string targetIp;
    int targetPort;
    int socketFd;
    struct sockaddr_in destAddr;
};