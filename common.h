#pragma once

#include <fstream>
#include <string>

inline std::string readFileContets(const char *file_path) {
  std::string file_contents{};

  std::ifstream file(file_path);

  if (file.is_open()) {
    std::string temp{};
    while (file) {
      std::getline(file, temp);
      if (temp != "") {
        file_contents += temp;
      }
    }
  } else {
    throw std::runtime_error("Could not open file");
  }

  return file_contents;
}
