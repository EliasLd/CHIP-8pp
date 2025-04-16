#ifndef CHIP8_CPU_HPP
#define CHIP8_CPU_HPP

#include <cstdint>
#include "constants.hpp"

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
public: 
    Cpu();

    void setPC(uint16_t value);

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