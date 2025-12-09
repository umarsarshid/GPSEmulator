#include "SerialPort.hpp"
#include "GPSDevice.hpp"
#include "ConfigLoader.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <csignal> // <--- NEW: For signal handling

// Global flag to control the loop
volatile std::sig_atomic_t keepRunning = 1;

void signalHandler(int signum) {
    keepRunning = 0;
}

void log(const std::string& msg) {
    auto now = std::time(nullptr);
    std::cout << "[" << std::put_time(std::localtime(&now), "%H:%M:%S") << "] " << msg << std::endl;
}

int main() {
    // Register the Ctrl+C handler
    std::signal(SIGINT, signalHandler);

    ConfigLoader config;
    if (!config.load("emulator.conf")) {
        std::cerr << "Warning: emulator.conf not found, using defaults." << std::endl;
    }

    // Use the symlink path by default now
    std::string portName = config.getString("PORT", "/tmp/ttyMock");
    int tickRate = config.getInt("TICK_RATE_MS", 100);

    SerialPort hw(portName);
    GPSDevice gps; 

    if (!hw.connect()) {
        log("FATAL: Failed to open serial port " + portName);
        return 1;
    }

    log("Service started on " + portName + ". Press Ctrl+C to stop.");

    auto lastTick = std::chrono::steady_clock::now();
    auto tickDuration = std::chrono::milliseconds(tickRate);

    // Use the flag instead of 'true'
    while (keepRunning) {
        auto now = std::chrono::steady_clock::now();
        if (now - lastTick >= tickDuration) {
            gps.updateState();
            std::string nmea = gps.getNMEASentence();
            
            static int logCounter = 0;
            if (++logCounter % 10 == 0) {
                 log("TX: " + nmea.substr(0, nmea.length()-2));
            }
            
            hw.send(nmea);
            lastTick = now;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    log("Shutting down safely...");
    return 0;
}
