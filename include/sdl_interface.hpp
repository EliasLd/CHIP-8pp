#ifndef CHIP8_SDL_INTERFACE
#define CHIP8_SDL_INTERFACE

#include <SDL.h>
#include "chip8.hpp"

class SdlInterface
{
private:
    SDL_Window* window      {nullptr};
    SDL_Renderer* renderer  {nullptr};
    SDL_Texture* texture    {nullptr};
    Chip8* system           {nullptr};


public:
    SdlInterface(const char* window_title,
                int window_width, int window_height,
                int texture_width, int texture_height, Chip8* system);
    ~SdlInterface();

    bool HandleKeyInput();
};

#endif
