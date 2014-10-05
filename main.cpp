#include <iostream>
#include <fstream>
#include "chip8.h"

int main(int argc, char *argv[]) {

    Chip8::Chip8 chip8;

    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <filename>\n" << std::endl;
        std::exit(2);
    }

    std::ifstream program;
    std::cout << "[main] Opening program" << std::endl;
    program.open(argv[1], std::ifstream::binary);
    if (!program.is_open()) {
        std::cerr << "couldn't open program" << std::endl;
        exit(1);
    }

    std::cout << "[main] Loading program" << std::endl;
    chip8.load_program(program);

    std::cout << "[main] Running program" << std::endl;
    chip8.run();
}
