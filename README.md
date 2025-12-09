# **Mock Hardware Emulator (GPS/NMEA-0183)**

A robust, configurable C++ daemon that simulates hardware devices for embedded software testing. It generates realistic NMEA-0183 GPS streams via a virtual serial port and includes a "Chaos Engine" to simulate real-world hardware failures like signal loss, data corruption, and latency.

## **Features**

* **Virtual Serial Interface:** Connects to client applications via socat virtual ports.  
* **Realistic Protocol:** Generates valid $GPGGA NMEA sentences with dynamic state (latitude/longitude drift) and correct checksums.  
* **10Hz Heartbeat:** Non-blocking, precise event loop independent of CPU speed.  
* **Hot-Configurable:** Change settings via emulator.conf without recompiling.  
* **Fault Injection (The Chaos Engine):**  
  * **Jitter:** Simulate network lag/thread blocking.  
  * **Corruption:** Randomly flip bits to test checksum validation.  
  * **Drop:** Simulate packet loss or loose cables.

## **Prerequisites**

* **OS:** Linux or macOS  
* **Compiler:** C++17 compatible (GCC or Clang)  
* **Build System:** CMake (3.10+)  
* **Tools:** socat (for creating virtual cable pairs)

### **Installing Dependencies**

**macOS:**  
brew install socat cmake

**Ubuntu/Debian:**  
sudo apt-get install socat cmake build-essential

## **Build Instructions**

1. **Clone the repository:** 
```bash 
   git clone https://github.com/umarsarshid/GPSEmulator.git
   cd MockHardwareEmulator
```
2. **Create build directory and compile:**  
 ```bash  
   mkdir build  
   cd build  
   cmake ..  
   make
```
## **⚡ Quick Start**

### **1\. Create the Virtual "Cable"**

Before running the emulator, you need to establish the virtual connection. We use socat to create two linked ports: /tmp/ttyMock (Server) and /tmp/ttyClient (Client).  
Run this in a separate terminal window and keep it open:  

```bash
socat \-d \-d pty,raw,echo=0,link=/tmp/ttyMock pty,raw,echo=0,link=/tmp/ttyClient
```
### **2\. Run the Emulator**

In your build directory:  
```bash
./emulator
```
*You should see logs indicating the service has started.*

### **3\. Connect a Client**

Open a third terminal to act as your "Dashboard App" or "Map Software":  
```bash
cat /tmp/ttyClient
```
*You should see a stream of $GPGGA data appearing 10 times a second.*

## **⚙️ Configuration**

The application reads from emulator.conf in the execution directory. If the file is missing, it defaults to internal values.  
**Example emulator.conf:**  
\# \--- Connection Settings \---  
\# The port the emulator writes to (must match your socat command)  
PORT=/tmp/ttyMock

\# \--- Simulation Settings \---  
\# Starting coordinates (Vancouver, BC)  
START\_LAT=49.2827  
START\_LON=-123.1207

\# Update frequency (milliseconds). 100ms \= 10Hz  
TICK\_RATE\_MS=100

\# \--- THE CHAOS ENGINE \---  
\# Probability of a fault occurring per packet (0.0 to 1.0)  
\# 0.0 \= Perfect connection  
\# 1.0 \= Total destruction  
CHAOS\_PROBABILITY=0.2

\# Maximum lag to introduce in milliseconds (simulates blocking I/O)  
JITTER\_MAX\_MS=500

## **Testing Resilience (Fault Injection)**

To test if your client application handles errors correctly, modify emulator.conf:

1. **Simulate Bad Cables:** Set CHAOS\_PROBABILITY=0.5. \~50% of packets will be dropped or corrupted.  
2. **Simulate Latency:** Set JITTER\_MAX\_MS=1000. You will see the timestamp update rate become erratic.  
3. **Verify Integrity:** Watch your client app.  
   * Does it crash when it receives $GP?GA instead of $GPGGA?  
   * Does it freeze when data stops flowing for 500ms?