#include "cpu.hpp"

Cpu::Cpu()
{
    // Initialize the programe counter
    setPC(Chip8Specs::ProgramStartAddress);
}

void Cpu::setPC(uint16_t value) { pc = value; }