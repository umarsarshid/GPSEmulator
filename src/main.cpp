#include "SerialPort.hpp"
#include "GPSDevice.hpp"
#include <iostream>
#include <thread> // For sleep
#include <chrono> // For seconds

int main(int argc, char* argv[]) {
    // Allow user to pass port name.
    // Default to a common Mac socat port if none provided.
    std::string portName = (argc > 1) ? argv[1] : "/dev/ttys023";
    SerialPort hw(portName);
    GPSDevice gps;

    if (!hw.connect()) {
        std::cerr << "Failed to connect to " << portName << std::endl;
        return 1;
    }

    std::cout << "Connected to " << portName << ". Streaming data..." << std::endl;

     // --- TIMING VARIABLES ---
    auto lastTick = std::chrono::steady_clock::now();
    // 10Hz = 100 milliseconds per tick
    const auto TICK_RATE = std::chrono::milliseconds(100); 
    std::cout << "Daemon started at 10Hz..." << std::endl;


    // The Real Loop
    while (true) {
        auto now = std::chrono::steady_clock::now();
        
        // If 100ms has passed since the last tick...
        if (now - lastTick >= TICK_RATE) {
            // 1. UPDATE STATE
            gps.updateState();
            
            // 2. SEND DATA
            std::string nmea = gps.getNMEASentence();
            hw.send(nmea);
            
            // 3. RESET CLOCK
            lastTick = now;
        }

        // Small sleep to prevent 100% CPU usage, but small enough to remain responsive
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}