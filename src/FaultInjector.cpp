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