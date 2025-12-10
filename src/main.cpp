#include "ITransport.hpp"   // <--- The Interface
#include "SerialPort.hpp"
#include "UDPTransport.hpp" // <--- The New Class
#include "GPSDevice.hpp"
#include "ConfigLoader.hpp"
#include "FaultInjector.hpp"
#include <memory>           // For std::unique_ptr
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

    // --- DYNAMIC TRANSPORT SELECTION ---
    std::string type = config.getString("TRANSPORT_TYPE", "SERIAL");
    
    // Use unique_ptr to manage the memory automatically
    std::unique_ptr<ITransport> hw;

    if (type == "UDP") {
        std::string ip = config.getString("UDP_IP", "127.0.0.1");
        int port = config.getInt("UDP_PORT", 10110);
        log("Configuring UDP Transport -> " + ip + ":" + std::to_string(port));
        
        hw = std::make_unique<UDPTransport>(ip, port);
    } else {
        std::string portName = config.getString("PORT", "/tmp/ttyMock");
        log("Configuring Serial Transport -> " + portName);
        
        hw = std::make_unique<SerialPort>(portName);
    }

    if (!hw->connect()) { // Arrow operator (->) because it's a pointer
        log("FATAL: Failed to connect transport.");
        return 1;
    }
    // -----------------------------------


    // Load Chaos Settings
    double chaosProb = config.getDouble("CHAOS_PROBABILITY", 0.0);
    int jitterMax = config.getInt("JITTER_MAX_MS", 0);
    int tickRate = config.getInt("TICK_RATE_MS", 100);

    GPSDevice gps; 

        // Initialize Injector
    FaultInjector injector;
    injector.setProbability(chaosProb);
    injector.setMaxJitter(jitterMax);

    auto lastTick = std::chrono::steady_clock::now();
    auto tickDuration = std::chrono::milliseconds(tickRate);

    // Use the flag instead of 'true'
    while (keepRunning) {
        auto now = std::chrono::steady_clock::now();
        if (now - lastTick >= tickDuration) {
            gps.updateState();
            std::string nmea = gps.getNMEASentence();

            // Apply faults
            bool dropped = injector.applyFaults(nmea);
              if (!dropped) {
                hw->send(nmea);
                
                // Logging logic (updated to show corruption)
                static int logCounter = 0;
                if (++logCounter % 10 == 0) {
                     log("TX: " + nmea.substr(0, nmea.length()-2));
                }
            } else {
                log("FAULT: Packet Dropped");
            }
            hw->send(nmea);
            lastTick = now;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    log("Shutting down safely...");
    return 0;
}
