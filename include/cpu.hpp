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
    // alias for pointer to a Cpu member function
    // of type void with no argument
    using CpuInstruction = void (Cpu::*)();
    // function pointer table. Will contain
    // references to instructions
    CpuInstruction table[0xF + 1] {};
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

    void opc_00E0();
    void opc_00EE();

    void opc_1nnn();
    void opc_2nnn();
    void opc_3xkk();
    void opc_4xkk();
    void opc_5xy0();
    void opc_9xy0();
    void opc_Bnnn();

    void opc_6xkk();
    void opc_7xkk();
    void opc_Annn();
    void opc_Fx1E();
    void opc_Fx55();
    void opc_Fx65();
    void opc_Fx33();
    void opc_Cxkk();

    void opc_Fx07();
    void opc_Fx15();
    void opc_Fx18();
    void opc_Ex9E();
    void opc_ExA1();
    void opc_Fx0A();
    void opc_Dxyn();
    void opc_Fx29();

    void dispatchInstructions();
    void handle0Instructions();
    void handle8Instructions();
    void handleEInstructions();
    void handleFInstructions();

    void Cycle();
    
};

#endif
