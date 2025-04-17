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

// Used to get Register X address value
uint8_t Cpu::extractVx(uint16_t mask)
{
    return (opcode & mask) >> 8u;
}
// Used to get Register Y address value
uint8_t Cpu::extractVy(uint16_t mask)
{
    return (opcode & mask) >> 4u;
}

// === Instructions ===

// Arithmetic and logical instructions
// between registers x and y

// LD vx, vy
void Cpu::opc_8xy0() 
{
    uint8_t vx {extractVx(MASK_OPC_VX)};
    uint8_t vy {extractVy(MASK_OPC_VY)};

    registers[vx] = registers[vy];
}

// OR vx, vy
void Cpu::opc_8xy1()
{
    uint8_t vx {extractVx(MASK_OPC_VX)};
    uint8_t vy {extractVy(MASK_OPC_VY)};

    registers[vx] |= registers[vy];
}

// AND vx, vy
void Cpu::opc_8xy2()
{
    uint8_t vx {extractVx(MASK_OPC_VX)};
    uint8_t vy {extractVy(MASK_OPC_VY)};

    registers[vx] &= registers[vy];
}

// XOR vx, vy
void Cpu::opc_8xy3()
{
    uint8_t vx {extractVx(MASK_OPC_VX)};
    uint8_t vy {extractVy(MASK_OPC_VY)};

    registers[vx] ^= registers[vy];
}

// ADD vx, vy
void Cpu::opc_8xy4()
{
    uint8_t vx {extractVx(MASK_OPC_VX)};
    uint8_t vy {extractVy(MASK_OPC_VY)};

    uint16_t sum { registers[vx] + registers[vy] };

    // Register vf will carry a flag is the sum overflows 255
    registers[0xF] = (sum > MASK_LOWER_8BITS) ? 1 : 0;

    registers[vx] = sum & MASK_LOWER_8BITS;
}