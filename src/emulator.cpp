#include <chrono>
#include <iomanip>
#include <iostream>

#include "chip8.hpp"
#include "cpu.hpp"
#include "sdl_interface.hpp"
#include "constants.hpp"

int main(int argc, char* argv[])
{
    if (argc != 4)
	{
		std::cerr << "Emulator Usage: " << argv[0] << " <ROM> <Scale> <Delay>" << '\n';
		std::exit(EXIT_FAILURE);
	}

	char* romFilename       { argv[1] };
	int video_scale_coeff   { std::stoi(argv[2]) };
	int cycle_delay         { std::stoi(argv[3]) };

    Chip8 chip8 {};

    try {
        chip8.loadRomIntoMemory(romFilename);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }
    SdlInterface interface("Chip8pp", Chip8Specs::ScreenWidth * video_scale_coeff, Chip8Specs::ScreenHeight * video_scale_coeff, Chip8Specs::ScreenWidth, Chip8Specs::ScreenHeight, &chip8);

    int pitch { static_cast<int>(sizeof(chip8.getVideo()[0]) * Chip8Specs::ScreenWidth) };

	auto previous_cycle_time { std::chrono::high_resolution_clock::now() };
	bool quit { false };

    while (!quit)
	{
		quit = interface.HandleKeyInput();

		auto current_time { std::chrono::high_resolution_clock::now() };
		float delta_time {
            std::chrono::duration<float, std::chrono::milliseconds::period>(current_time - previous_cycle_time).count()
        };

		if (delta_time > cycle_delay)
		{
			previous_cycle_time = current_time;

			chip8.Cycle();

            if(chip8.getSoundTimer() > 0) interface.PlaySound();

			interface.Update(pitch);
		}
	}

    return 0;
}
