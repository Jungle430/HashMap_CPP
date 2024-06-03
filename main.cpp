#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/std.h>

#include <map>

#include "HashMap.hpp"
#include "display.h"

constexpr int NUM_COUNT = 50000;

auto hashmap() noexcept -> void {
  JAVA::HashMap<std::string, std::string> h1;

  for (int i = 0; i < NUM_COUNT; i++) {
    h1.Put(std::to_string(i), std::to_string(i + 1));
  }

  for (int i = 0; i < NUM_COUNT; i++) {
    h1.Contain(std::to_string(i));
  }
  fmt::println("{}", h1);
}

auto stdmap() noexcept -> void {
  std::map<std::string, std::string> m;

  for (int i = 0; i < NUM_COUNT; i++) {
    m[std::to_string(i)] = std::to_string(i + 1);
  }

  for (int i = 0; i < NUM_COUNT; i++) {
    m.find(std::to_string(i));
  }

  // fmt::println("{}", m);
}

auto main() -> int {
  JAVA::display();
  JAVA::HashMap<int, int> h1;
  std::map<int, int> map;

  auto start = std::chrono::high_resolution_clock::now();
  hashmap();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  fmt::println("{}", elapsed.count());

  start = std::chrono::high_resolution_clock::now();
  stdmap();
  end = std::chrono::high_resolution_clock::now();
  elapsed = end - start;
  fmt::println("{}", elapsed.count());

  return 0;
}