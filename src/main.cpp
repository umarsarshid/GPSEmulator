#include "SerialPort.cpp"
#include "GPSDevice.cpp"
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



    // The Real Loop
    while (true) {
        std::string msg = "ping\n";
        hw.send(msg);
        std::cout << "TX: " << msg;
        gps.updateState();
        std::string nmea = gps.getNMEASentence() + "\n";
        std::cout << "NMEA: " << nmea;
        // Sleep for 1 second to simulate 1Hz hardware
        std::this_thread::sleep_for(std::chrono::seconds(1));

    }

    return 0;
}