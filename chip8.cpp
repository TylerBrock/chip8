#include "unistd.h"
#include "chip8.h"

/**
 * 0x000-0x1FF Chip 8 interpreter
 * 0x050-0x0A0 4x5 pixel font set
 * 0x200-0xFFF Program ROM and work RAM
 */

namespace Chip8 {

    Chip8::Chip8() {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        _graphics.reset((new Graphics));
        _memory.reset((new Memory));
        _cpu.reset((new CPU(_graphics.get(), _memory.get())));
    }

    Chip8::~Chip8() {
        SDL_Quit();
    }

    void Chip8::load_program(std::ifstream& program) {
        program.seekg(0, std::ios::end);
        int size = program.tellg();

        program.seekg(0, std::ios::beg);
        for (int i=0; i<size; i++) {
            _memory->putByte(0x200 + i, program.get());
        }
    }

    void Chip8::run() {
        bool quit = false;
        SDL_Event event;

        while (!quit) {
            _cpu->run_cycle();

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    quit = true;
                if (event.type == SDL_KEYDOWN)
                    quit = true;
            }

            _graphics->refresh();

            SDL_Delay(100);
        }
    }

} // namespace Chip8

