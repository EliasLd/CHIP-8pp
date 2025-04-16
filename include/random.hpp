#ifndef CHIP8_RANDOM_HPP
#define CHIP8_RANDOM_HPP

#include <random>
#include <chrono>
#include <cstdint>

// === Header only class ===
/*
    Used to generate an 8-bit random
    number (values from 0 to 255)
*/

class RandomGenerator
{
private:
    std::mt19937 mt;
    std::uniform_int_distribution<uint8_t> dist;
public:
    RandomGenerator()
    {
        std::random_device rd;
        std::seed_seq ss {
            static_cast<std::seed_seq::result_type>(
                std::chrono::steady_clock::now().time_since_epoch().count()),
                rd(), rd(), rd(), rd(), rd(), rd(), rd()
        };

        mt = std::mt19937 { ss };
        dist = std::uniform_int_distribution<uint8_t> { 0, 255U };
    }

    uint8_t get() { return dist(mt); }
};

#endif
