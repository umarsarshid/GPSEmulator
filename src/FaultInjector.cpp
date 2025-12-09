#include "FaultInjector.hpp"
#include <thread>
#include <chrono>
#include <iostream>

FaultInjector::FaultInjector() : probability(0.0), maxJitterMs(0) {
    // Seed the random engine with hardware entropy
    std::random_device rd;
    rng.seed(rd());
}

void FaultInjector::setProbability(double prob) {
    probability = prob;
}

void FaultInjector::setMaxJitter(int ms) {
    maxJitterMs = ms;
}

bool FaultInjector::shouldFault() {
    std::uniform_real_distribution<> dist(0.0, 1.0);
    return dist(rng) < probability;
}


bool FaultInjector::applyFaults(std::string& packet) {
    if (!shouldFault()) return false; // No faults this time

    // Roll a die to decide WHICH fault to apply
    std::uniform_int_distribution<> typeDist(0, 2);
    int faultType = typeDist(rng);

    switch (faultType) {
        case 0: // DROP
            // We return 'true' to signal the caller to SKIP sending this packet
            return true; 
        
        case 1: // CORRUPTION
            corrupt(packet);
            break;

        case 2: // JITTER
            jitter();
            break;
    }
    return false; // Packet exists (maybe corrupted/delayed), but not dropped
}

void FaultInjector::corrupt(std::string& packet) {
    if (packet.length() < 5) return; // Too short to mess with
    
    // Pick a random index (avoiding the $ at start or \r\n at end if possible)
    std::uniform_int_distribution<> idxDist(1, packet.length() - 3);
    int idx = idxDist(rng);

    // Flip a bit or change char
    // Let's replace a valid number/comma with a random garbage char like '#' or '?'
    packet[idx] = '?'; 
}

void FaultInjector::jitter() {
    if (maxJitterMs <= 0) return;
    
    std::uniform_int_distribution<> timeDist(10, maxJitterMs);
    int delay = timeDist(rng);
    
    // BLOCK the thread to simulate lag
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}
