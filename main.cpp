#include <benchmark/benchmark.h>

#include <map>
#include <unordered_map>

#include "../include/HashMap.hpp"
#include "display.h"

constexpr int NUM_COUNT = 5000000;

auto HashMapBenchmark() noexcept -> void {
  JAVA::HashMap<int, int> h1;

  for (int i = 0; i < NUM_COUNT; i++) {
    h1.Put(i, i + 1);
  }

  for (int i = 0; i < NUM_COUNT; i++) {
    h1.Contain(i);
  }

  for (int i = NUM_COUNT - 1; i >= 0; i--) {
    h1.Del(i);
  }

  JAVA::HashMap<std::string, std::string> h2;
  for (int i = 0; i < NUM_COUNT; i++) {
    h2.Put(std::to_string(i), std::to_string(i + 1));
  }

  for (int i = 0; i < NUM_COUNT; i++) {
    h2.Contain(std::to_string(i));
  }

  for (int i = NUM_COUNT - 1; i >= 0; i--) {
    h2.Del(std::to_string(i));
  }
}

auto StdUnorderedMapBenchmark() noexcept -> void {
  std::unordered_map<int, int> h1;

  for (int i = 0; i < NUM_COUNT; i++) {
    h1[i] = i + 1;
  }

  for (int i = 0; i < NUM_COUNT; i++) {
    h1.count(i);
  }

  for (int i = NUM_COUNT - 1; i >= 0; i--) {
    h1.erase(i);
  }

  std::unordered_map<std::string, std::string> h2;

  for (int i = 0; i < NUM_COUNT; i++) {
    h2[std::to_string(i)] = std::to_string(i + 1);
  }

  for (int i = 0; i < NUM_COUNT; i++) {
    h2.count(std::to_string(i));
  }

  for (int i = NUM_COUNT - 1; i >= 0; i--) {
    h2.erase(std::to_string(i));
  }
}

auto StdMapBenchmark() noexcept -> void {
  std::map<int, int> h1;

  for (int i = 0; i < NUM_COUNT; i++) {
    h1[i] = i + 1;
  }

  for (int i = 0; i < NUM_COUNT; i++) {
    h1.count(i);
  }

  for (int i = NUM_COUNT - 1; i >= 0; i--) {
    h1.erase(i);
  }

  std::map<std::string, std::string> h2;

  for (int i = 0; i < NUM_COUNT; i++) {
    h2[std::to_string(i)] = std::to_string(i + 1);
  }

  for (int i = 0; i < NUM_COUNT; i++) {
    h2.count(std::to_string(i));
  }

  for (int i = NUM_COUNT - 1; i >= 0; i--) {
    h2.erase(std::to_string(i));
  }
}

static void CustomHashMapBenchmark(benchmark::State& state) {
  for (auto _ : state) {
    HashMapBenchmark();
  }
}

static void CustomStdUnorderedMapBenchmark(benchmark::State& state) {
  for (auto _ : state) {
    StdUnorderedMapBenchmark();
  }
}

static void CustomStdMapBenchmark(benchmark::State& state) {
  for (auto _ : state) {
    StdMapBenchmark();
  }
}

BENCHMARK(CustomHashMapBenchmark);
BENCHMARK(CustomStdUnorderedMapBenchmark);
BENCHMARK(CustomStdMapBenchmark);

auto main(int argc, char** argv) -> int {
  JAVA::display();
  // NOLINTNEXTLINE(modernize-avoid-c-arrays)
  char arg0_default[] = "benchmark";
  char* args_default = arg0_default;
  if (argv == nullptr) {
    argc = 1;
    argv = &args_default;
  }
  ::benchmark::Initialize(&argc, argv);
  if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
    return 1;
  }
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return 0;
}