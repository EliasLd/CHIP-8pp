#ifndef CHIP8_CPU_HPP
#define CHIP8_CPU_HPP

#include <cstdint>
#include "constants.hpp"

class Cpu {
private:
    uint8_t registers[Chip8Specs::RegisterCount] {};
    // Program counter, holds the address of
    // the next instruction to execute
    uint16_t pc {};
    // keeps track of the order of execution
    uint16_t stack[Chip8Specs::StackDepth] {};
    // stack pointer, keeps track of the most
    // recent value placed in the stack
    uint8_t sp {};
public: 
    Cpu();
};

#endif