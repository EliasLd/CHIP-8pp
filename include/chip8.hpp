#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>

#include "constants.hpp"
#include "cpu.hpp"
#include "random.hpp"

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
    RandomGenerator random_device {};
    Cpu cpu {};
public:
    Chip8();

    void loadRomIntoMemory(const std::string& filename);

    uint32_t* getVideo();
    uint8_t* getKeypad();
    uint16_t getIndexRegister();
    uint8_t getMemoryAt(uint8_t index);
    uint8_t getDelayTimer();
    uint8_t getSoundTimer();

    void setIndexRegister(uint16_t value);
    void writeMemory(uint8_t index, uint8_t value);
    void setDelayTimer(uint8_t value);
    void setSoundTimer(uint8_t value);

};

#endif
