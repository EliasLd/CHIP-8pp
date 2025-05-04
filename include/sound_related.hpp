#ifndef SOUND_SPECS_HPP
#define SOUND_SPECS_HPP

#include <SDL.h>

namespace SoundSpecs 
{
    // frequency in Hz
    constexpr int device_frequency { 44100 };
    // 8 unsigned bits audio
    SDL_AudioFormat device_format { AUDIO_U8 };
    // audio buffer size
    constexpr int device_samples { 1024 };

    constexpr int sound_frequency { 1000 };
    constexpr int sound_duration_ms { 100 }; 
}

#endif
