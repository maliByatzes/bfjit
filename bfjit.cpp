#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

const int MEM_SIZE{30'000};

enum class Token {
  INC = '+',
  DEC = '-',
  LEFT = '<',
  RIGHT = '>',
  OUTPUT = '.',
  INPUT = ',',
  JUMP_FORWARD = '[',
  JUMP_BACKWARD = ']'
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

std::vector<Command> convertToIR(const std::string &contents,
                                 const char *file_path) {
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
    } break;
    case ']': {
      if (addr_stack.size() == 0) {
        std::cerr << file_path << " [" << index
                  << "]: ERROR: Unbalanced loops.\n";
        return {};
      }

      std::size_t addr{addr_stack.top()};
      addr_stack.pop();
      Command cmd{.token = static_cast<Token>(current_char), .operand = addr};
      commands.push_back(cmd);
      commands.at(addr).operand = commands.size();
    } break;
    default: {
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

  std::vector<Command> commands{convertToIR(file_contets, file_path)};

  std::vector<uint8_t> memory(MEM_SIZE, 0);
  std::size_t ip{0};
  std::size_t idx{0};

  while (idx < commands.size()) {
    Command cmd{commands.at(idx)};
    switch (cmd.token) {
    case Token::INC: {
      memory.at(ip) += cmd.operand;
      idx++;
    } break;
    case Token::DEC: {
      memory.at(ip) -= cmd.operand;
      idx++;
    } break;
    case Token::LEFT: {
      if (ip < cmd.operand) {
        std::cerr << "RUNTIME ERROR: Memory underflow.\n";
        return 1;
      }
      ip -= cmd.operand;
      idx++;
    } break;
    case Token::RIGHT: {
      if (ip >= memory.size()) {
        memory.resize(MEM_SIZE * 2);
      }
      ip += cmd.operand;
      idx++;
    } break;
    case Token::OUTPUT: {
      while (cmd.operand > 0) {
        std::cout << static_cast<char>(memory.at(ip));
        cmd.operand--;
      }
      idx++;
    } break;
    case Token::INPUT: {
      assert(0 && "TODO: INPUT is not implmented");
    } break;
    case Token::JUMP_FORWARD: {
      if (memory.at(ip) == 0) {
        idx = cmd.operand;
      } else {
        idx++;
      }
    } break;
    case Token::JUMP_BACKWARD: {
      if (memory.at(ip) != 0) {
        idx = cmd.operand;
      } else {
        idx++;
      }
    } break;
    default: {
    }
    }
  }

  return 0;
}
