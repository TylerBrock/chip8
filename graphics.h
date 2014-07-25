#pragma once

#include <cstdint>
#include "SDL2/SDL.h"

namespace Chip8 {

    class Graphics {
    public:
        Graphics();
        ~Graphics();

        void set(int x, int y, bool value);
        bool get(int x, int y) const;
        bool dirty() const;
        void clear();
        void refresh();

    private:
        bool gfx[64 * 32];
        bool _dirty_buffer;
        SDL_Window* _window;
        SDL_Renderer* _renderer;
    };

} // namespace Chip8
