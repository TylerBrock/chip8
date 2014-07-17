#include <iostream>
#include <iomanip>
#include "memory.h"

namespace Chip8 {

    const uint16_t kFontSetLocation = 0x050;

    const uint8_t kFontSet[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    Memory::Memory() {
        for (int i=0; i<(kMemorySize); i++)
            memory[i] = 0;

        for (int i=0; i<16; i++) {
            memory[kFontSetLocation + i] = kFontSet[i];
        }
    }

    uint8_t Memory::getByte(int location) {
        std::cout << "accessing 0x" << std::hex << location << std::endl;
        return memory[location];
    }

    void Memory::putByte(int location, uint8_t value) {
        memory[location] = value;
    }

    uint16_t Memory::getFontLocation() {
        return kFontSetLocation;
    }

    void Memory::dump() {
        for (int i = 0; i < (kMemorySize / 16); i++) {
            // Print the header
            if (i%16 == 0) {
                std::cout << "      ";
                for (int j=0; j<16; j++) {
                    std::cout << j << "  ";
                }
                std::cout << std::endl;
            }

            // Print out 256 byte block in 16 16-byte rows
            std::cout << "0x" << std::setfill('0') << std::setw(3) << std::hex << i*16;

            // Print raw memory for this row
            for (int j = 0; j < 16; j++)
                std::cout << " " << std::hex << std::setw(2) << int(memory[(i*16) + j]);
            std::cout << std::endl;

            if (i%16 == 15)
                std::cout << std::endl;
        }
        std::cout << std::endl;
    }

} // namespace Chip8
