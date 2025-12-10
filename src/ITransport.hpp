#pragma once
#include <string>

// Abstract Base Class (Interface)
class ITransport {
public:
    virtual ~ITransport() = default;
    
    // Pure virtual functions - children MUST implement these
    virtual bool connect() = 0;
    virtual int send(const std::string& data) = 0;


};
