#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/std.h>

#include "HashMap.hpp"
#include "display.h"

auto main() -> int {
  JAVA::display();
  JAVA::HashMap<std::string, int> h1;
  h1.Put(std::string("xiaoming"), 1);
  h1.Put(std::string("xiaohong"), 2);

  fmt::println("{}", h1);

  JAVA::HashMap<std::size_t, int> h2;
  for (std::size_t i = 0; i < 200; i++) {
    h2.Put(i, static_cast<int>(i + 1));
  }
  fmt::println("{}", h2);

  return 0;
}