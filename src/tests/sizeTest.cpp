#include <gtest/gtest.h>

#include "../include/HashMap.hpp"

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
TEST(sizeAndPutTest, AssertionTrue) {
  JAVA::HashMap<int, int> h1;
  // NOLINTNEXTLINE(readability-magic-numbers)
  for (int i = 0; i < 100000; i++) {
    h1.Put(i, i + 1);
    ASSERT_EQ(h1.size(), i + 1);
  }

  // NOLINTNEXTLINE(readability-magic-numbers)
  for (int i = 0; i < 50000; i++) {
    h1.Del(i);
  }
  ASSERT_EQ(h1.size(), 100000 - 50000);

  // NOLINTNEXTLINE(readability-magic-numbers)
  for (int i = 50000; i < 100000; i++) {
    h1.Del(i);
  }
  ASSERT_TRUE(h1.empty());
}

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
TEST(emptyTest, AssertionTrue) {
  JAVA::HashMap<int, int> h1;
  ASSERT_TRUE(h1.empty());
  // NOLINTNEXTLINE(readability-magic-numbers)
  for (int i = 0; i < 100000; i++) {
    h1.Put(i, i + 1);
    ASSERT_FALSE(h1.empty());
  }
}

auto main(int argc, char **argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::FLAGS_gtest_output = "stdout:";
  return RUN_ALL_TESTS();
}
