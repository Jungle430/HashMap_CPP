#include <gtest/gtest.h>

#include "../include/HashMap.hpp"

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
TEST(ContainTest, AssertionTrue) {
  JAVA::HashMap<int, int> h1;
  // NOLINTNEXTLINE(readability-magic-numbers)
  for (int i = 0; i < 100000; i++) {
    h1.Put(i, i + 1);
  }

  // NOLINTNEXTLINE(readability-magic-numbers)
  for (int i = 0; i < 100000; i++) {
    ASSERT_TRUE(h1.Contain(i));
  }

  // NOLINTNEXTLINE(readability-magic-numbers)
  for (int i = 100000; i < 200000; i++) {
    ASSERT_FALSE(h1.Contain(i));
  }
}

auto main(int argc, char **argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::FLAGS_gtest_output = "stdout:";
  return RUN_ALL_TESTS();
}