#include "sdl_interface.hpp"
#include "sound_related.hpp"
#include "constants.hpp"

#include <iostream>

SdlInterface::SdlInterface(const char* window_title,
    int window_width, int window_height,
    int texture_width, int texture_height, Chip8* system) : system {system}
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    window = SDL_CreateWindow(window_title, 0, 0, window_width, window_height, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 
        texture_width, texture_height);

    InitSound();
}

SdlInterface::~SdlInterface()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(audio_device);
    delete[] audio_buffer;

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

void SdlInterface::Update(int pitch)
{
    uint32_t frame_buffer[Chip8Specs::ScreenWidth * Chip8Specs::ScreenHeight];

    uint32_t* video { system->getVideo() };

    for(int i {} ; i < Chip8Specs::ScreenWidth * Chip8Specs::ScreenHeight ; ++i)
        frame_buffer[i] = (video[i] ? Chip8Specs::ColorOn : Chip8Specs::ColorOff);

    SDL_UpdateTexture(texture, nullptr, frame_buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void SdlInterface::InitSound()
{
    SDL_AudioSpec spec;
    SDL_zero(spec);

    spec.freq = SoundSpecs::device_frequency;
    spec.format = SoundSpecs::device_format;
    // mono channel
    spec.channels = 1;
    spec.samples = SoundSpecs::device_samples;
    // No audio callback
    spec.callback = nullptr;

    audio_device = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
    if (audio_device == 0)
    {
        std::cerr << "SDL audio error: " << SDL_GetError() << std::endl;
        return;
    }

    // Sound generation

    const int frequency { SoundSpecs::sound_frequency };
    const int duration_ms { SoundSpecs::sound_duration_ms };

    const int sample_count { spec.freq * duration_ms / 1000 };
    audio_length = sample_count;

    audio_buffer = new uint8_t[sample_count];

    int half_period = spec.freq / (2 * frequency);
    // Square wave: 255 (high), 0 (low)
    for (int i = 0 ; i < sample_count ; ++i)
        audio_buffer[i] = (i / half_period) % 2 == 0 ? 255 : 0;
}

void SdlInterface::PlaySound()
{
    SDL_ClearQueuedAudio(audio_device);
    SDL_QueueAudio(audio_device, audio_buffer, audio_length);
    SDL_PauseAudioDevice(audio_device, 0);
}
