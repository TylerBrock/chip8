#include <iostream>
#include <fstream>
#include "chip8.h"

int main() {
    Chip8::Chip8 chip8;
    std::ifstream program;
    std::cout << "[main] Opening program" << std::endl;
    program.open("PONG", std::ifstream::binary);
    if (!program.is_open()) {
        std::cerr << "couldn't open program" << std::endl;
        exit(1);
    }
    std::cout << "[main] Loading program" << std::endl;
    chip8.load_program(program);
    std::cout << "[main] Running program" << std::endl;
    chip8.run();
}
