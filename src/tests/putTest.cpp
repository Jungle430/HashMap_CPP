#include <gtest/gtest.h>

#include "../include/HashMap.hpp"

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
TEST(PutTest, AssertionTrue) {
  JAVA::HashMap<int, int> h;
  h.Put(1, 1);
}

auto main(int argc, char **argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
