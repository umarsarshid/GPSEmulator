#include "GPSDevice.hpp"
#include <cstdio> // for snprintf
#include <numeric> // for std::accumulate if I wanted to be fancy, but loop is easier
#include <iomanip>
#include <sstream>

GPSDevice::GPSDevice() : latitude(49.2827), longitude(-123.1207), altitude(10.0), updateCount(0) {
    // Starting near Vancouver, BC
}

void GPSDevice::updateState() {
    // Simulate a slow drift north-east
    latitude += 0.0001; 
    longitude += 0.0001;
    updateCount++;
}

std::string GPSDevice::getNMEASentence() {
    char buffer[100];
    
    // 1. Construct the payload WITHOUT '$' or '*'
    // Real NMEA uses a weird format (Degrees + Minutes), but let's stick to basics first.
    // I need to make sure I use positive numbers for the string format (hence -longitude)
    //calculate the length of the formatted string
    int len = snprintf(buffer, sizeof(buffer), 
        "GPGGA,123519,%0.4f,N,%0.4f,W,1,08,0.9,%.1f,M,5.0,M,,", 
        latitude, -longitude, altitude);

    std::string payload(buffer, len);

    // 2. Calculate Checksum (The "Secret Sauce")
    //NMEA checksum is XOR of all characters between $ and *
    unsigned char checksum = 0;
    for (char c : payload) {
        checksum ^= c; // XOR every character
    }

    // 3. Format into Hex
    std::stringstream ss;
    ss << "$" << payload << "*" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)checksum << "\r\n";
    
    return ss.str();
}
