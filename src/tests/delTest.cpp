#include <gtest/gtest.h>

#include "../include/HashMap.hpp"

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
TEST(DelTest, AssertionTrue) {
  JAVA::HashMap<int, int> h1;
  // NOLINTNEXTLINE(readability-magic-numbers)
  for (int i = 0; i < 10000; i++) {
    h1.Put(i, i + 1);
  }

  // NOLINTNEXTLINE(readability-magic-numbers)
  for (int i = 0; i < 10000; i++) {
    if (i % 2 == 0) {
      h1.Del(i);
    }
  }

  // NOLINTNEXTLINE(readability-magic-numbers)
  for (int i = 0; i < 10000; i++) {
    if (i % 2 == 0) {
      ASSERT_FALSE(h1.Contain(i));
    } else {
      ASSERT_TRUE(h1.Contain(i));
    }
  }
}

auto main(int argc, char **argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::FLAGS_gtest_output = "stdout:";
  return RUN_ALL_TESTS();
}