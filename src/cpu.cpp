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
    Used for clear the display and return
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

// Memory & Registers instructions

// LD vx, byte
void Cpu::opc_6xkk()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };
    uint8_t byte { static_cast<uint8_t>(opcode & MASK_OPC_BYTE) };

    registers[vx] = byte;
}

// ADD vx, byte
void Cpu::opc_7xkk()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };
    uint8_t byte { static_cast<uint8_t>(opcode & MASK_OPC_BYTE) };

    registers[vx] += byte;
}

// LD I, addr
void Cpu::opc_Annn()
{
    uint16_t address { static_cast<uint16_t>(opcode & MASK_OPC_ADDR) };

    system->setIndexRegister(address);
}

// ADD I, vx
void Cpu::opc_Fx1E()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };

    system->setIndexRegister(system->getIndexRegister() + registers[vx]);
}

// LD I, vx
void Cpu::opc_Fx55()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };

    for(uint8_t i {} ; i < vx ; ++i)
        system->writeMemory(system->getIndexRegister() + i, registers[i]);
}

// LD vx, I
void Cpu::opc_Fx65()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };

    for(uint8_t i {} ; i < vx ; ++i)
        registers[i] = system->getMemoryAt(system->getIndexRegister() + i);
}

// LD B, vx
void Cpu::opc_Fx33()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };
    uint8_t val { registers[vx] };

    uint16_t index { system->getIndexRegister() };
    // ones digit
    system->writeMemory(index, val % 10);
    val /= 10;

    // tens digit
    system->writeMemory(index + 1, val % 10);
    val /= 10;

    // hundreds digit
    system->writeMemory(index + 2, val % 10);
}

// RND vx, byte
void Cpu::opc_Cxkk()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };
    uint8_t byte { static_cast<uint8_t>(opcode & MASK_OPC_BYTE) };

    registers[vx] = system->getRandomByte() & byte;
}

/*
    System & Peripheral instructions
    including timers, keypad and sprite
    manipulation
*/

// LD vx, DT
void Cpu::opc_Fx07()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };

    registers[vx] = system->getDelayTimer();
}

// LD DT, vx
void Cpu::opc_Fx15()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };

    system->setDelayTimer(registers[vx]);
}

// LD ST, vx
void Cpu::opc_Fx18()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };

    system->setSoundTimer(registers[vx]);
}

// SKP vx
void Cpu::opc_Ex9E()
{
    uint8_t vx  { extractVx(MASK_OPC_VX) };
    uint8_t key { registers[vx] };

    if(key < Chip8Specs::KeysCount && system->getKeypad()[key])
        pc += 2;
}

// SKNP vx
void Cpu::opc_ExA1()
{
    uint8_t vx  { extractVx(MASK_OPC_VX) };
    uint8_t key { registers[vx] };

    if(key < Chip8Specs::KeysCount && !system->getKeypad()[key])
        pc += 2;
}

// LD vx, K
// Wait for a key press
void Cpu::opc_Fx0A()
{
    uint8_t vx  { extractVx(MASK_OPC_VX) };
    uint8_t* keypad { system->getKeypad() };

    for(uint8_t i {} ; i < Chip8Specs::KeysCount ; ++i)
    {
        if(keypad[i])
        {
            registers[vx] = i;
            return;
        }
    }

    pc -= 2;
}

// DRW vx, vy, nibble
// Display a sprite of size n at (vx, vy)
// starting at memory location index_register
void Cpu::opc_Dxyn()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };
    uint8_t vy { extractVy(MASK_OPC_VY) };
    uint8_t sprite_height { static_cast<uint8_t>(opcode & MASK_OPC_NIBBLE) };

    // Wrap to avoid overflow screen boundaries
    uint8_t x_cord = registers[vx] % Chip8Specs::ScreeHeight;
    uint8_t y_cord = registers[vy] % Chip8Specs::ScreeHeight;

    registers[0xF] = 0;

    for(uint row {} ; row < sprite_height ; ++row)
    {
        uint8_t sprite_byte { system->getMemoryAt(system->getIndexRegister() + row) };

        for(uint col {}  ; col < Chip8Specs::SpriteWidth ; ++col)
        {
            uint8_t sprite_pixel = sprite_byte & (0x80u >> col) ;
            uint32_t* video { system->getVideo() };
            uint32_t* screen_pixel { &video[(y_cord + row) * Chip8Specs::ScreenWidth + (x_cord + col)] };

            if(sprite_pixel)
            {
                if(*screen_pixel == Chip8Specs::PixelOn)
                {
                    // sprite pixel and screen pixel are both on
                    // -> there's a collision
                    registers[0xF] = 1;
                }

                *screen_pixel ^= Chip8Specs::PixelOn;
            }
        }
    }
}

// LD F, vx
void Cpu::opc_Fx29()
{
    uint8_t vx { extractVx(MASK_OPC_VX) };
    uint8_t digit { registers[vx] };

    // font characters are 5 bytes long
    uint16_t sprite_first_char_location = Chip8Specs::FontSetStartAddress + (Chip8Specs::FontCharSize * digit);

    system->setIndexRegister(sprite_first_char_location);
}