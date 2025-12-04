cmake_minimum_required(VERSION 3.10)
project(MockHardwareEmulator)

set(CMAKE_CXX_STANDARD 17)

# I'm separating the logic (SerialPort) from the runner (main)
# so I can test them separately later.
add_executable(emulator src/main.cpp)
