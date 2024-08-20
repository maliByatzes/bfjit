#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

enum class Token {
    TK_INC = '+',
    TK_DEC = '-',
    TK_LEFT = '<',
    TK_RIGHT = '>',
    TK_OUTPUT = '.',
    TK_INPUT = ',',
    TK_JUMP_FORWARD = '[',
    TK_JUMP_BACKWARD = ']'
};

struct Command {
    Token token {};
    std::size_t operand;
};

std::string readFileContets(const char* file_path) {
    std::string file_contets{};

    std::ifstream file(file_path);

    if (file.is_open()) {
       std::string temp{};
       while (file) {
           std::getline(file, temp);
           if (temp != "") {
               file_contets += temp;
           }
       }
    } else {
        throw std::runtime_error("Could not open file");
    }

    return file_contets;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Not enough arguments.\n";
        std::cout << "Usage: " << argv[0] << " <input.bf>\n";
        return 1;
    }

    const char* file_path{ argv[1] };

    std::string file_contets{};
    try {
        file_contets = readFileContets(file_path);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::cout << "File contents: " << file_contets << '\n';

    return 0;
}
