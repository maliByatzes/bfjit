#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

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
  Token token{};
  std::size_t operand;
};

std::string readFileContets(const char *file_path) {
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

std::vector<Command> convertToIR(const std::string &contents, const char* file_path) {
  std::vector<Command> commands{};
  std::stack<std::size_t> addr_stack{};

  for (std::size_t index = 0; index < contents.size() - 1; ++index) {
    char current_char{contents.at(index)};
    switch (current_char) {
    case '+':
    case '-':
    case '>':
    case '<':
    case '.':
    case ',': {
      std::size_t count{1};
      while (contents.at(index + 1) == current_char) {
        count++;
        current_char = contents.at(index++);
      }
      Command cmd{.token = static_cast<Token>(current_char), .operand = count};
      commands.push_back(cmd);
    } break;
    case '[': {
        std::size_t addr{commands.size()};
        Command cmd{.token = static_cast<Token>(current_char), .operand = 0};
        commands.push_back(cmd);
        addr_stack.push(addr);
        current_char = contents.at(index++);
    } break;
    case ']': {
        if (addr_stack.size() == 0) {
            std::cerr << file_path << " [" << index << "]: ERROR: Unbalanced loops.\n";
            return {};
        }

        std::size_t addr { addr_stack.top() };
        addr_stack.pop();
        Command cmd{.token=static_cast<Token>(current_char), .operand = addr + 1};
        commands.push_back(cmd);
        commands.at(addr).operand = commands.size();
        current_char = contents.at(index++);
    } break;
    default: {
        std::cerr << "ERROR: Invalid character.\n";
        return {};
    }
    }
  }

  return commands;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Not enough arguments.\n";
    std::cout << "Usage: " << argv[0] << " <input.bf>\n";
    return 1;
  }

  const char *file_path{argv[1]};

  std::string file_contets{};
  try {
    file_contets = readFileContets(file_path);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  std::vector<Command> commands{ convertToIR(file_contets, file_path) };

  for (std::size_t idx=0; idx < commands.size(); ++idx) {
      std::cout << idx << ": "<< static_cast<char>(commands.at(idx).token) << ": " << commands.at(idx).operand << '\n';
  }

  return 0;
}
