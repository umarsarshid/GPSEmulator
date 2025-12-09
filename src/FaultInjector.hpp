#pragma once
#include <string>
#include <random>

class FaultInjector {
public:
    FaultInjector();
    
    // Returns true if the packet was fully dropped (simulating cable disconnect)
    bool applyFaults(std::string& packet);

    void setProbability(double prob); // 0.0 to 1.0
    void setMaxJitter(int ms);

private:
    std::mt19937 rng;
    double probability;
    int maxJitterMs;

    // Helpers
    bool shouldFault();
    void corrupt(std::string& packet);
    void jitter();
};
