#include "UDPTransport.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

UDPTransport::UDPTransport(const std::string& ip, int port) 
    : targetIp(ip), targetPort(port), socketFd(-1) {
}

UDPTransport::~UDPTransport() {
    if (socketFd != -1) close(socketFd);
}

bool UDPTransport::connect() {
    // 1. Create Socket (IPv4, Datagram/UDP, Protocol 0)
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd < 0) {
        perror("UDP Socket creation failed");
        return false;
    }

    // 2. Setup Destination Address
    std::memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(targetPort); // Host to Network Short
    
    // Convert IP string (127.0.0.1) to binary
    if (inet_pton(AF_INET, targetIp.c_str(), &destAddr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address: " << targetIp << std::endl;
        return false;
    }

    return true;
}

int UDPTransport::send(const std::string& data) {
    if (socketFd == -1) return -1;

    // UDP sendto
    return sendto(socketFd, data.c_str(), data.size(), 0, 
          (struct sockaddr*)&destAddr, sizeof(destAddr));
}