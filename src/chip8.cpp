#include "chip8.hpp"

Chip8::Chip8()
{
    // Load all fonts into memory
    for(uint i {} ; i < Chip8Specs::FontsetSize; ++i)
    {
        memory[Chip8Specs::FontSetStartAddress + i] = Chip8Specs::FontSet[i];
    }
}