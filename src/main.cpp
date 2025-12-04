#include "SerialPort.cpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // 1. Hardcoded port for quick debugging
    // NOTE: On Mac, check socat output for exact number (e.g., ttys003)
    SerialPort hw("/dev/ttys015");
    
    if (!hw.connect()) {
        std::cerr << "Error: Failed to connect to serial port." << std::endl;
        return 1;
    }

    // 2. The Test Case
    std::cout << "Sending 'hello' (no newline)..." << std::endl;
    hw.send("hello"); 
    
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Sending 'hello\\n' (with newline)..." << std::endl;
    hw.send("hello\n");

    return 0;
}
