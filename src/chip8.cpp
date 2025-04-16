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