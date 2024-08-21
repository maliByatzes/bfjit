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

std::size_t calculateAddress(const std::vector<Command> &commands) {
  std::size_t counter{};
  for (auto const &cmd : commands) {
    if (cmd.token == Token::JUMP_FORWARD || cmd.token == Token::JUMP_BACKWARD) {
      counter++;
    } else {
      counter += cmd.operand;
    }
  }
  return counter;
}

std::vector<Command> convertToIR(const std::string &contents,
                                 const char *file_path) {
  std::vector<Command> commands{};
  std::stack<std::pair<std::size_t, std::size_t>> addr_stack{};

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
      std::size_t addr{calculateAddress(commands)};
      Command cmd{.token = static_cast<Token>(current_char), .operand = 0};
      commands.push_back(cmd);
      std::size_t cmd_index{commands.size() - 1};
      addr_stack.push({cmd_index, addr + 2});
    } break;
    case ']': {
      if (addr_stack.size() == 0) {
        std::cerr << file_path << " [" << index
                  << "]: ERROR: Unbalanced loops.\n";
        return {};
      }

      std::pair<std::size_t, std::size_t> addr{addr_stack.top()};
      addr_stack.pop();
      Command cmd{.token = static_cast<Token>(current_char),
                  .operand = addr.second};
      commands.push_back(cmd);
      commands.at(addr.first).operand = calculateAddress(commands);
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

  std::vector<Command> commands{convertToIR(file_contets, file_path)};

  std::vector<uint8_t> memory(MEM_SIZE, 0);
  std::size_t ip{0};

  for (std::size_t idx = 0; idx < commands.size(); ++idx) {
    switch (commands.at(idx).token) {
    case Token::INC: {
      memory.at(ip) += commands.at(idx).operand;
    } break;
    case Token::DEC: {
      memory.at(ip) -= commands.at(idx).operand;
    } break;
    case Token::LEFT: {
      if (ip < 0) {
        std::cerr << "RUNTIME ERROR: Memory underflow.\n";
        return 1;
      }
      ip--;
    } break;
    case Token::RIGHT: {
      if (ip > MEM_SIZE) {
        memory.resize(MEM_SIZE * 2);
      }
      ip++;
    } break;
    case Token::OUTPUT: {
      std::cout << static_cast<char>(memory.at(ip));
    } break;
    case Token::INPUT: {
      assert(0 && "TODO: INPUT is not implmented");
    } break;
    case Token::JUMP_FORWARD: {
      if (memory.at(ip) == 0) {
        ip = commands.at(idx).operand + 1;
      } else {
        ip++;
      }
    } break;
    case Token::JUMP_BACKWARD: {
      if (memory.at(ip) != 0) {
        ip = commands.at(idx).operand;
      } else {
        ip++;
      }
    } break;
    default: {
      std::cerr << "ERROR: Invalid command.\n";
      return 1;
    }
    }
  }

  return 0;
}
