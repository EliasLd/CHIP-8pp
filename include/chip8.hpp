#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>

#include "constants.hpp"
#include "cpu.hpp"

class Chip8
{
private:
    // RAM
    uint8_t memory[Chip8Specs::MemorySize] {};
    // Special register used to store memory addresses
    uint16_t index_register {};
    uint8_t delay_timer {};
    uint8_t sound_timer {};
    uint8_t keypad[Chip8Specs::KeysCount] {};
    // 1D array representing a 2D screen
    uint32_t video[Chip8Specs::ScreenWidth * Chip8Specs::ScreeHeight] {};
    // Operation code, represents an instruction that has
    // to be executed by the cpu
    uint16_t opcode {};
    Cpu cpu {};
public:
    Chip8();
};

#endif