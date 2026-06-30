#ifndef RNGH
#define RNGH

#include <random>
#include <cstdint>

// Minimal random number generator: wraps std::mt19937 and yields a double
// in [0, 1). Construct with a seed.
struct RNG {
    std::mt19937 gen;
    explicit RNG(uint32_t seed) : gen(seed) {}
    // returns a double in [0, 1)
    double operator()() {
        return std::uniform_real_distribution<double>(0.0, 1.0)(gen);
    }
};

#endif
