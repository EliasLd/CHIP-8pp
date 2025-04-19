#include "cpu.hpp"
#include "chip8.hpp"
#include "masks.hpp"

#include <cstring>

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

    uint16_t sum { static_cast<uint16_t>(registers[vx] + registers[vy]) };

    // Register vf will carry a flag is the sum overflows 255
    registers[0xF] = (sum > MASK_LOWER_8BITS) ? 1 : 0;

    registers[vx] = sum & MASK_LOWER_8BITS;
}

// SUB vx, vy
void Cpu::opc_8xy5()
{
    uint8_t vx {extractVx(MASK_OPC_VX)};
    uint8_t vy {extractVy(MASK_OPC_VY)};

    // Register vf is set to 1 if vx > vy
    registers[0xF] = (registers[vx] > registers[vy]) ? 1 : 0;

    registers[vx] -= registers[vy];
}

// SHR vx
void Cpu::opc_8xy6()
{
    uint8_t vx {extractVx(MASK_OPC_VX)};

    // Save least significant bit 
    registers[0xF] = (registers[vx] & MASK_LSB);

    registers[vx] >>= 1;
}

// SUBN vx, vy
void Cpu::opc_8xy7()
{
    uint8_t vx {extractVx(MASK_OPC_VX)};
    uint8_t vy {extractVy(MASK_OPC_VY)};

    registers[0xF] = (registers[vy] > registers[vx]) ? 1 : 0;

    registers[vx] = registers[vy] - registers[vx];
}

// SHL vx
void Cpu::opc_8xyE()
{
    uint8_t vx {extractVx(MASK_OPC_VX)};

    registers[0xF] = (registers[vx] & MASK_MSB) >> 7u;

    registers[vx] <<= 1;
}

// Machine instructions

/*
    historically called system instructions
    Used for clear the displaya and return 
    from an instruction
*/

// CLS
void Cpu::opc_00E0()
{
    uint32_t* video { system->getVideo() };
    constexpr size_t buffer_size { Chip8Specs::ScreenWidth * Chip8Specs::ScreeHeight };
    std::memset(video, 0, buffer_size * sizeof(uint32_t));
}

// RET
void Cpu::opc_00EE()
{
    pc = stack[--sp];
}

/*
    Flow control instructions
    directly influences program's flow
    (jump, subroutine call, skip...)
*/

// JP addr
void Cpu::opc_1nnn()
{
    uint16_t address { static_cast<uint16_t>(opcode & MASK_OPC_ADDR) };
    pc = address;
}

// CALL addr
void Cpu::opc_2nnn()
{
    uint16_t address { static_cast<uint16_t>(opcode & MASK_OPC_ADDR) };

    stack[sp] = pc;
    ++sp;
    pc = address;
}

// SE vx, byte
// Skip instruction if vx = kk
void Cpu::opc_3xkk()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };
    uint8_t byte { static_cast<uint8_t>(opcode & MASK_OPC_BYTE) };

    if(registers[vx] == byte) pc += 2;
}

// SNE vx, byte
// Skip instruction if vx != kk
void Cpu::opc_4xkk()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };
    uint8_t byte { static_cast<uint8_t>(opcode & MASK_OPC_BYTE) };

    if(registers[vx] != byte) pc += 2;
}

// SE vx, vy
// Skip instruction if vx = vy
void Cpu::opc_5xy0()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };
    uint8_t vy { extractVy(MASK_OPC_VY) };

    if(registers[vx] == registers[vy]) pc += 2;
}

// SNE vx, vy
// Skip instruction if vx != vy
void Cpu::opc_9xy0()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };
    uint8_t vy { extractVy(MASK_OPC_VY) };

    if(registers[vx] != registers[vy]) pc += 2;
}

// JP addr, v0
void Cpu::opc_Bnnn()
{
    uint16_t address { static_cast<uint16_t>(opcode & MASK_OPC_ADDR) };

    pc = registers[0] + address;
}
