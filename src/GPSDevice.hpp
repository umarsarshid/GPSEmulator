#pragma once
#include <string>

class GPSDevice {
public:
    GPSDevice();
    
    // Call this every "tick" to move the car/boat slightly
    void updateState();
    
    // Get the current raw NMEA string
    std::string getNMEASentence();

private:
    double latitude;
    double longitude;
    double altitude;
    int updateCount; // Just to make things change
};