#pragma once

#include <cstdint>

/**
 * 0xF?? - 0xFFF built in 4x5 pixel font set A-F, 1-9
 * 0x200 - 0xF?? program rom and work ram
 * 0x000 - 0x200 interpreter (reserved)
 */

namespace Chip8 {

    const int kMemorySize = 4 * 1024;

    class Memory {
    public:
        Memory();

        uint8_t getByte(int location);
        void putByte(int location, uint8_t value);
        uint16_t getFontLocation();
        void dump();

    private:
        uint8_t memory[kMemorySize];
    };

} // namespace Chip8
