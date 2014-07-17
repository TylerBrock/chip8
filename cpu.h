#pragma once

#include <cstdint>

namespace Chip8 {

    typedef uint16_t OpCode;
    typedef uint16_t Address;
    typedef uint8_t Constant;
    typedef uint8_t Register;

    class Graphics;
    class Memory;

    class CPU {
    public:
        CPU(Graphics* g, Memory* m);

        void run_cycle();
        void dump();
        void reset();

    private:
        uint8_t _registers[16];
        uint16_t _index_register;
        uint16_t _program_counter;
        //uint16_t _stack[16];
        uint16_t _stack_pointer;
        uint8_t _delay_timer;
        uint8_t _sound_timer;

        Graphics* _g;
        Memory* _m;
    };

} // namespace Chip8
