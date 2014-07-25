#include <iostream>
#include "graphics.h"

namespace Chip8 {

    const int kGraphicsWidth = 64;
    const int kGraphicsHeight = 32;
    const int kGraphicsScale = 10;

    Graphics::Graphics() {
        _window = SDL_CreateWindow(
            "Chip8",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            kGraphicsWidth * kGraphicsScale,
            kGraphicsHeight * kGraphicsScale,
            SDL_WINDOW_SHOWN
        );

        if (_window == nullptr)
            std::cout << "error: " << SDL_GetError() << std::endl;

        _renderer = SDL_CreateRenderer(
            _window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );

        if (_window == nullptr)
            std::cout << "error: " << SDL_GetError() << std::endl;

        _dirty_buffer = false;

        clear();
    }

    Graphics::~Graphics() {
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
    }

    void Graphics::set(int x, int y, bool value) {
        //std::cout << "[graphics] settingx: " << x << " settingy: " << y << std::endl;
        gfx[(kGraphicsWidth * y) + x - 1] = value;

        _dirty_buffer = true;
    }

    bool Graphics::get(int x, int y) const {
        return gfx[(kGraphicsWidth * y) + x - 1];
    }

    bool Graphics::dirty() const {
        return _dirty_buffer;
    }

    void Graphics::refresh() {
        if (SDL_RenderClear(_renderer) != 0) {
            std::cout << "[graphics] clear error: " << SDL_GetError() << std::endl;
        }

        for (int pixel = 0; pixel < kGraphicsWidth * kGraphicsHeight; pixel++) {

            SDL_Rect scaled_pixel = {
                (pixel % kGraphicsWidth) * kGraphicsScale,
                (pixel / kGraphicsWidth) * kGraphicsScale,
                kGraphicsScale,
                kGraphicsScale
            };

            if (gfx[pixel]) {
                SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
            }

            SDL_RenderFillRect(_renderer, &scaled_pixel);
        }

        _dirty_buffer = false;

        SDL_RenderPresent(_renderer);
    }

    void Graphics::clear() {
        // TODO: use memset
        for (int i=0; i<kGraphicsWidth * kGraphicsHeight; i++)
            gfx[i] = false;

        _dirty_buffer = true;
    }

} // namespace Chip8
