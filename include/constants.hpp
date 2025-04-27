#ifndef CHIP8_CONSTANTS_HPP
#define CHIP8_CONSTANTS_HPP

#include <cstdint>
#include <SDL.h>
#include <unordered_map>

namespace Chip8Specs
{
    // === Hardware specifications ===
    constexpr int MemorySize    {4096};
    constexpr int RegisterCount {16};
    constexpr int StackDepth    {16};
    constexpr int ScreenWidth   {64};
    constexpr int ScreenHeight   {32};
    constexpr int KeysCount     {16};

    // === Memory Mapping === 
    constexpr uint16_t FontSetStartAddress {0x050};
    constexpr uint16_t ProgramStartAddress {0x200};

    // === Pixels ===
    constexpr uint32_t PixelOn  {0xFFFFFFFF};
    constexpr uint32_t PixelOff {0x00000000};
    constexpr uint32_t ColorOn  {0xFF666600};
    constexpr uint32_t ColorOff {0xFFFFFFFF};

    // === Sprite ===
    constexpr int SpriteWidth {8};
    
    // === Fontset ===
    constexpr unsigned int FontsetSize  {80};
    constexpr unsigned int FontCharSize {5};
    constexpr uint8_t FontSet[FontsetSize]
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Keys code
    const std::unordered_map<SDL_Keycode, uint8_t> KeyMap {
        {SDLK_x, 0x0}, {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3},
        {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_a, 0x7},
        {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_z, 0xA}, {SDLK_c, 0xB},
        {SDLK_4, 0xC}, {SDLK_r, 0xD}, {SDLK_f, 0xE}, {SDLK_v, 0xF},
    };
}

#endif
