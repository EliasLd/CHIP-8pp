#ifndef CHIP8_CONSTANTS_HPP
#define CHIP8_CONSTANTS_HPP

#include <cstdint>

namespace Chip8Specs
{
    // Hardware defined specifications
    constexpr int MemorySize    = 4096;
    constexpr int RegisterCount = 16;
    constexpr int StackDepth    = 16;
    constexpr int ScreenWidth   = 64;
    constexpr int ScreeHeight   = 32;
    constexpr int KeysCount     = 16;
}

#endif