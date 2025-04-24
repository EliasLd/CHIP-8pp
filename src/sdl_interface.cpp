#include "sdl_interface.hpp"
#include "constants.hpp"

SdlInterface::SdlInterface(const char* window_title,
    int window_width, int window_height,
    int texture_width, int texture_height, Chip8* system) : system {system}
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(window_title, 0, 0, window_width, window_height, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 
        texture_width, texture_height);
}

SdlInterface::~SdlInterface()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool SdlInterface::HandleKeyInput()
{
    bool quit {false};

    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            // Get the pressed key
            SDL_Keycode pressed_key { event.key.keysym.sym };

            if(Chip8Specs::KeyMap.count(pressed_key) > 0)
            {
                uint8_t chip8_key { Chip8Specs::KeyMap.at(pressed_key) };
                system->setKeypad(chip8_key, (event.type == SDL_KEYUP ? 0 : 1));
            }

            break;
        }
    }

    return quit;
}
