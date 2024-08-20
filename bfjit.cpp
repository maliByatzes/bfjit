#include <cstddef>
#include <iostream>

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

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Not enough arguments.\n";
        std::cout << "Usage: " << argv[0] << " <input.bf>\n";
        return 1;
    }

    const char* file_path{ argv[1] };
    std::cout << "File path: " << file_path << '\n';
    return 0;
}
