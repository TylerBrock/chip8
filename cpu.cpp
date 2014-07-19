#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "cpu.h"
#include "graphics.h"
#include "memory.h"

namespace Chip8 {

    const uint16_t kOpCodeMask = 0xF000;
    const uint16_t kAddressMask = 0x0FFF;
    const uint16_t kRegisterXMask = 0x0F00;
    const uint16_t kRegisterYMask = 0x00F0;
    const uint16_t kImmediateMask = 0x00FF;
    const uint16_t kLastNibble = 0x000F;

    CPU::CPU(Graphics* g, Memory* m) : _g(g), _m(m) {
        reset();
    }

    void CPU::reset() {
        _program_counter = 0x200;
        _delay_timer = 0;
        _sound_timer = 0;
        _stack_pointer = 0;
        _index_register = 0;
        for (int i=0; i<16; i++) {
            _registers[i] = 0;
        }
    }

    void CPU::run_cycle() {
        // Decode instruction
        dump();
        OpCode op = _m->getByte(_program_counter) << 8 | _m->getByte(_program_counter + 1);
        std::cout << "Operation at 0x" << std::hex << _program_counter << " -> "
            << std::setw(4) << int(op) << std::endl;

        Address a = op & kOpCodeMask;
        uint8_t rx = (op & kRegisterXMask) >> 8;
        uint8_t ry = (op & kRegisterYMask) >> 4;
        uint8_t last_byte = op & 0x0FF;

        // Run instruction
        switch (op & kOpCodeMask) {
            case 0x0000:
                if (op == 0x00E0)
                    // Clear the screen
                    _g->clear();
                else if (op == 0x00EE)
                    // Return from subroutine
                    _program_counter = _stack_pointer;
                else
                    _m->dump();
                break;

            case 0x1000:
                _program_counter = a;
                break;

            case 0x2000:
                _stack_pointer = _program_counter + 2;
                _program_counter = op & 0x0FFF;
                break;

            case 0x3000:
                if (rx == (op & kImmediateMask))
                    _program_counter += 4;
                else
                    _program_counter += 2;
                break;

            case 0x4000:
                if (rx != (op & kImmediateMask))
                    _program_counter += 4;
                else
                    _program_counter += 2;
                break;

            case 0x5000:
                if (rx == ry)
                    _program_counter += 4;
                else
                    _program_counter += 2;
                break;

            case 0x6000:
                _registers[rx] = (op & kImmediateMask);
                _program_counter += 2;
                break;

            case 0x7000:
                _registers[rx] += (op & kImmediateMask);
                _program_counter += 2;
                break;

            case 0x8000:
                switch (op & kLastNibble) {
                    case 0x0:
                        // Sets VX to the value of VY
                        _registers[rx] = _registers[ry];
                        break;
                    case 0x1:
                        // Sets VX to VX OR VY
                        _registers[rx] |= _registers[ry];
                        break;
                    case 0x2:
                        // Sets VX to VX AND VY
                        _registers[rx] &= _registers[ry];
                        break;
                    case 0x3:
                        // Sets VX to VX XOR VY
                        _registers[rx] ^= _registers[ry];
                        break;
                    case 0x4:
                        // Adds VY to VX, VF is set to carry
                        _registers[rx] += _registers[ry];
                        break;
                    case 0x5:
                        // VY is subtracted from VX, VF is set to zero when there is a borrow
                        _registers[rx] -= _registers[ry];
                        break;
                    case 0x6:
                        // Shifts VX right by one, VF set to least significant bit before shift
                        _registers[rx] >>= 1;
                        break;
                    case 0x7:
                        // Sets VX to VY minus VX, VF is set to zero when there is a borrow
                        _registers[rx] = _registers[ry] - _registers[rx];
                        break;
                    case 0xE:
                        // Shifts VX left by one, VF set to most significant bit before shift
                        _registers[rx] <<= 1;
                        break;
                }
                _program_counter += 2;
                break;

            case 0xA000:
                _index_register = (op & kAddressMask);
                _program_counter += 2;
                break;

            case 0xC000:
                _registers[rx] = std::rand() & (op & kImmediateMask);
                _program_counter += 2;
                break;

            case 0xD000: {
                int x = _registers[(op & kRegisterXMask) >> 8];
                int y = _registers[(op & kRegisterYMask) >> 4];
                int n = (op & kLastNibble);

                uint16_t sprite_pointer = _index_register;

                // Height is determined by the last nibble
                for (int i=0; i < n; i++) {

                    // Chip8 sprites are ALWAYS 8 pixels wide
                    uint8_t sprite_mask = 0x80;
                    uint8_t sprite_row = _m->getByte(sprite_pointer + i);

                    std::cout << "[cpu] Current sprite pointer: " << (int)sprite_pointer + i << std::endl;
                    std::cout << "[cpu] Current sprite row: " << (int)sprite_row << std::endl;

                    for (int j=0; j<8; j++) {
                        if (sprite_row & sprite_mask)
                            _g->set(x + j, y + i, true);
                        sprite_mask >>= 1;
                    }
                }
                _program_counter += 2;
                break;
            }

            case 0xE000:
                switch (last_byte) {
                    //case 0xA1:
                        //uint8_t key = _registers[rx];
                }
                _program_counter += 2;
                break;

            case 0xF000:
                int mod = 100;
                switch (last_byte) {
                    case 0x07:
                        _registers[rx] = _delay_timer;
                        break;

                    case 0x15:
                        _delay_timer = _registers[rx];
                        break;

                    case 0x29:
                        // set index register to location of font for vx
                        _index_register = _m->getFontLocation() + _registers[rx];
                        break;

                    case 0x33:
                        for (int i=0; i<3; i++) {
                            _m->putByte(_index_register + i, _registers[rx] % mod);
                            mod /= 10;
                        }
                        break;

                    case 0x65:
                        for (int i=0; i<=rx; i++)
                            _registers[i] = _m->getByte(_index_register + i);
                        break;
                }
                _program_counter += 2;
        }
    }

    void CPU::dump() {
        std::cout << "-------- CPU Registers --------" << std::endl;
        for (int i=0; i<4; i++) {
            std::cout << std::hex << std::setfill('0') <<
                std::setw(1) << i + 0  << ": 0x" << std::setw(2) << int(_registers[i+0])  << " " <<
                std::setw(1) << i + 4  << ": 0x" << std::setw(2) << int(_registers[i+4])  << " " <<
                std::setw(1) << i + 8  << ": 0x" << std::setw(2) << int(_registers[i+8])  << " " <<
                std::setw(1) << i + 12 << ": 0x" << std::setw(2) << int(_registers[i+12]) << " " <<
                std::endl;
        }
        std::cout << std::endl;

        std::cout << "pc: 0x" << std::setw(3) << int(_program_counter) << std::endl
                  << "ir: 0x" << std::setw(3) << int(_index_register) << std::endl
                  << "sp: 0x" << std::setw(3) << int(_stack_pointer) << std::endl
                  << "dt: 0x" << std::setw(2) << int(_delay_timer) << std::endl
                  << "st: 0x" << std::setw(2) << int(_sound_timer) << std::endl;

        std::cout << "-------------------------------" << std::endl;
        std::cout << std::endl;
    }

} // namespace Chip8
