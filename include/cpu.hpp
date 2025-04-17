#ifndef CHIP8_CPU_HPP
#define CHIP8_CPU_HPP

#include <cstdint>
#include "constants.hpp"

class Chip8;

class Cpu {
private:
    uint8_t registers[Chip8Specs::RegisterCount] {};
    // Program counter, holds the address of
    // the next instruction to execute
    uint16_t pc {};
    // keeps track of the order of execution
    uint16_t stack[Chip8Specs::StackDepth] {};
    // stack pointer, keeps track of the most
    // recent value placed in the stack
    uint8_t sp {};
    // Operation code, represents an instruction that has
    // to be executed by the cpu
    uint16_t opcode {};
    // Reference to the Chip8 system
    // used to simplify memory access
    Chip8* system {nullptr};
public: 
    Cpu();

    void setSystem(Chip8* sys);
    void setPC(uint16_t value);

    uint8_t extractVx(uint16_t mask);
    uint8_t extractVy(uint16_t mask);

    // Instructions
    void opc_8xy0();
    void opc_8xy1();
    void opc_8xy2();
    void opc_8xy3();
    void opc_8xy4();
    void opc_8xy5();
    void opc_8xy6();
    void opc_8xy7();
    void opc_8xyE();

};

#endif