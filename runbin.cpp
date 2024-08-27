#include "common.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char **argv) {
    if (argc != 2) {
      std::cerr << "Not enough arguments.\n";
      std::cout << "Usage: " << argv[0] << " <input.bin>\n";
      return 1;
    }

    const char *file_path{argv[1]};
    std::string file_contents{};
    try {
       file_contents = readFileContets(file_path);
    } catch(std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::cout << file_contents.size() << '\n';
}
