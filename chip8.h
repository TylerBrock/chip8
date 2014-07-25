#pragma once

#include <iostream>
#include <cstdint>
#include <fstream>
#include <memory>

#include "SDL2/SDL.h"

#include "cpu.h"
#include "memory.h"
#include "graphics.h"
#include "audio.h"

namespace Chip8 {

    class Chip8 {
    public:
        Chip8();
        ~Chip8();

        void load_program(std::ifstream& program);
        void run();

    private:
        std::unique_ptr<Graphics> _graphics;
        std::unique_ptr<Memory> _memory;
        std::unique_ptr<CPU> _cpu;
        std::unique_ptr<Audio> _audio;
    };

} // namespace Chip8
