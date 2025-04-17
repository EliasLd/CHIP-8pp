#include "cpu.hpp"
#include "chip8.hpp"
#include "masks.hpp"

Cpu::Cpu()
{
    // Initialize the program counter
    setPC(Chip8Specs::ProgramStartAddress);
}

void Cpu::setSystem(Chip8* sys) { system = sys; }
void Cpu::setPC(uint16_t value) { pc = value; }

// === Instructions ===

// Arithmetic and logical instructions
// between registers x and y

// Set registers: vx = vy
void Cpu::opc_8xy0() 
{
    uint8_t vx = (opcode & MASK_OPC_VX) >> 8u;
    uint8_t vy = (opcode & MASK_OPC_VY) >> 4u;

    registers[vx] = registers[vy];
}