#include "chip8.hpp"

#include <fstream>
#include <stdexcept>

Chip8::Chip8()
{
    // Load all fonts into memory
    for(uint i {} ; i < Chip8Specs::FontsetSize; ++i)
    {
        memory[Chip8Specs::FontSetStartAddress + i] = Chip8Specs::FontSet[i];
    }

    // Bind the system to the cpu
    cpu.setSystem(this);
}

// Accessors
uint32_t* Chip8::getVideo() { return video; }
uint8_t* Chip8::getKeypad() { return keypad; }
uint16_t Chip8::getIndexRegister() { return index_register; }
uint8_t Chip8::getMemoryAt(uint8_t index) { return memory[index]; }
uint8_t Chip8::getDelayTimer() { return delay_timer; }
uint8_t Chip8::getSoundTimer() { return sound_timer; }
uint8_t Chip8::getRandomByte() { return random_device.get(); }

// Mutators
void Chip8::setIndexRegister(uint16_t value) { index_register = value; }
void Chip8::writeMemory(uint8_t index, uint8_t value) { memory[index] = value; }
void Chip8::setDelayTimer(uint8_t value) { delay_timer = value; }
void Chip8::setSoundTimer(uint8_t value) { sound_timer = value; }

void Chip8::loadRomIntoMemory(const std::string& filename)
{
    // Open the file in binary mode
    std::ifstream rom(filename, std::ios::binary);
    if(!rom.is_open())
        throw std::runtime_error("Error: failed to open ROM : " + filename);

    // Get the size of the rom file
    rom.seekg(0, std::ios::end);
    std::streamsize rom_size = static_cast<std::streamsize>(rom.tellg());
    rom.seekg(0, std::ios::beg);

    // Dump rom content into a buffer
    char* buffer = new char[rom_size];
    rom.read(buffer, rom_size);

    // Copy rom content into memory
    for(std::streamsize i {} ; i < rom_size ; ++i)
        memory[Chip8Specs::ProgramStartAddress + i] = buffer[i];
}

void Chip8::Cycle()
{
    cpu.Cycle();

    if(delay_timer > 0) --delay_timer;
    if(sound_timer > 0) --sound_timer;
}
