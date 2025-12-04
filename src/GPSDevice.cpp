#include "GPSDevice.hpp"
#include <cstdio> // for snprintf

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
    // Format: $GPGGA,Time,Lat,N,Lon,E,Fix,Sats,HDOP,Alt,M...
    char buffer[100];
    
    // Note: This is a simplified format. Real NMEA uses ddmm.mmmm logic.
    // For now, I just want to see numbers appear.
    snprintf(buffer, sizeof(buffer), 
        "$GPGGA,123519,%0.4f,N,%0.4f,W,1,08,0.9,%.1f,M", 
        latitude, -longitude, altitude);
        
    return std::string(buffer);
}
