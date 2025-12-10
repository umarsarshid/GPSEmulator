cmake_minimum_required(VERSION 3.10)
project(MockHardwareEmulator)

set(CMAKE_CXX_STANDARD 17)

# I'm separating the logic (SerialPort) from the runner (main)
# so I can test them separately later.
# Add GPSDevice.cpp to the executable
# Add ConfigLoader.cpp to the source list
add_executable(emulator 
    src/main.cpp 
    src/SerialPort.cpp 
    src/UDPTransport.cpp
    src/GPSDevice.cpp 
    src/ConfigLoader.cpp
    src/FaultInjector.cpp
)

# AUTOMATICALLY COPY CONFIG:
# This takes 'emulator.conf' from the source root and copies it to
# the 'build' folder whenever we run 'make'.
configure_file(emulator.conf ${CMAKE_CURRENT_BINARY_DIR}/emulator.conf COPYONLY)
