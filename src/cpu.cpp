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
    constexpr size_t buffer_size { Chip8Specs::ScreenWidth * Chip8Specs::ScreenHeight };
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
    uint8_t x_cord = registers[vx] % Chip8Specs::ScreenWidth;
    uint8_t y_cord = registers[vy] % Chip8Specs::ScreenHeight;

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

// === Instruction dispatching & opcode decoding ===
/*
    A pointer table is used to correctly
    bind opcode and instructions
*/

void Cpu::dispatchInstructions()
{
    table[0x0] = &Cpu::handle0Instructions;
    table[0x1] = &Cpu::opc_1nnn;
    table[0x2] = &Cpu::opc_2nnn;
    table[0x3] = &Cpu::opc_3xkk;
    table[0x4] = &Cpu::opc_4xkk;
    table[0x5] = &Cpu::opc_5xy0;
    table[0x6] = &Cpu::opc_6xkk;
    table[0x7] = &Cpu::opc_7xkk;
    table[0x8] = &Cpu::handle8Instructions;
    table[0x9] = &Cpu::opc_9xy0;
    table[0xA] = &Cpu::opc_Annn;
    table[0xB] = &Cpu::opc_Bnnn;
    table[0xC] = &Cpu::opc_Cxkk;
    table[0xD] = &Cpu::opc_Dxyn;
    table[0xE] = &Cpu::handleEInstructions;
    table[0xF] = &Cpu::handleFInstructions;
}

void Cpu::handle0Instructions()
{
    switch(opcode & 0x00FFu)
    {
    case 0xE0: return opc_00E0();
    case 0xEE: return opc_00EE();
    default: break;
    }
}

void Cpu::handle8Instructions()
{
    switch (opcode & 0x000Fu)
    {
    case 0x0: return opc_8xy0();
    case 0x1: return opc_8xy1();
    case 0x2: return opc_8xy2();
    case 0x3: return opc_8xy3();
    case 0x4: return opc_8xy4();
    case 0x5: return opc_8xy5();
    case 0x6: return opc_8xy6();
    case 0x7: return opc_8xy7();
    case 0xE: return opc_8xyE();
    default: break;
    }
}

void Cpu::handleEInstructions()
{
    switch (opcode & 0x00FFu)
    {
    case 0x9E: return opc_Ex9E();
    case 0xA1: return opc_ExA1();
    default: break;
    }
}

void Cpu::handleFInstructions()
{
    switch (opcode & 0x00FFu)
    {
    case 0x07: return opc_Fx07();
    case 0x0A: return opc_Fx0A();
    case 0x15: return opc_Fx15();
    case 0x18: return opc_Fx18();
    case 0x1E: return opc_Fx1E();
    case 0x29: return opc_Fx29();
    case 0x33: return opc_Fx33();
    case 0x55: return opc_Fx55();
    case 0x65: return opc_Fx65();
    default: break;
    }
}

void Cpu::Cycle()
{
    // Fetch opcode from memory
    opcode = (system->getMemoryAt(pc) << 8u) | system->getMemoryAt(pc + 1);

    pc += 2;

    // Decode the instruction
    uint8_t instruction_id = (opcode & 0xF000u) >> 12;

    // Execute
    (this->*table[instruction_id])();
}
