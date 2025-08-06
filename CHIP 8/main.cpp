#include <iostream>
#include <chrono>
#include "Graphics.h"
#include "chip.h"


#undef main 

int main(int argc, char** argv) {
    if (argc != 4) {
		std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

	int videoScale = std::stoi(argv[1]);
	int cycleDelay = std::stoi(argv[2]);
	char const* romFilename = argv[3];

	Graphics graphics("CHIP-8 Emulator", 64 * videoScale, 32 * videoScale, 64, 32);

	Chip8 chip;
	chip.loadrom(romFilename);

	int videoPitch = sizeof(chip.display[0]) * 64;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit)
	{
		quit = graphics.ProcessInput(chip.keys);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (dt > cycleDelay) {
			lastCycleTime = currentTime;
			chip.machine_cycle();
			graphics.Update(chip.display, videoPitch);
		}
	}
	return 0;
}
