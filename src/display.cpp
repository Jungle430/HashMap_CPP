#include "include/display.h"

#include <fstream>
#include <ios>
#include <iostream>
#include <string>

auto JAVA::display() noexcept(false) -> void {
  std::ifstream display_file(JAVA::DISPALY_FILE_PATH);

  if (!display_file.is_open()) {
    throw std::ios_base::failure(std::string("Failed to open the file, path:") +
                                 JAVA::DISPALY_FILE_PATH);
  }

  std::string content((std::istreambuf_iterator<char>(display_file)),
                      std::istreambuf_iterator<char>());
  std::cout << content;
}