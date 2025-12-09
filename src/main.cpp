#include "SerialPort.hpp"
#include "GPSDevice.hpp"
#include "ConfigLoader.hpp" // <--- NEW
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip> // For time formatting

// Helper for logging
void log(const std::string& msg) {
    auto now = std::time(nullptr);
    std::cout << "[" << std::put_time(std::localtime(&now), "%H:%M:%S") << "] " << msg << std::endl;
}

int main() {
    // 1. Load Configuration
    ConfigLoader config;
    if (!config.load("emulator.conf")) {
        std::cerr << "Warning: emulator.conf not found, using defaults." << std::endl;
    }

    std::string portName = config.getString("PORT", "/dev/ttys003");
    int tickRate = config.getInt("TICK_RATE_MS", 100);

    // 2. Initialize Hardware with Config
    // (Note: I would need to update GPSDevice constructor to accept Lat/Lon, skipping for brevity)
    SerialPort hw(portName);
    GPSDevice gps; 

    if (!hw.connect()) {
        log("FATAL: Failed to open serial port " + portName);
        return 1;
    }

    log("Service started on " + portName + " at " + std::to_string(tickRate) + "ms tick.");

    // 3. The Loop
    auto lastTick = std::chrono::steady_clock::now();
    auto tickDuration = std::chrono::milliseconds(tickRate);

    while (true) {
        auto now = std::chrono::steady_clock::now();
        if (now - lastTick >= tickDuration) {
            gps.updateState();
            std::string nmea = gps.getNMEASentence();
            
            // Only log every 10th packet to avoid spamming the console at 10Hz
            static int logCounter = 0;
            if (++logCounter % 10 == 0) {
                 log("TX: " + nmea.substr(0, nmea.length()-2)); // Trim \r\n for log
            }
            
            hw.send(nmea);
            lastTick = now;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
