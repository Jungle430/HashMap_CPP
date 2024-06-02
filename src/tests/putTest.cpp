#include <fmt/format.h>
#include <gtest/gtest.h>

#include "../include/HashMap.hpp"

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
TEST(PutTest, AssertionTrue) {
  JAVA::HashMap<int, int> h;
  h.Put(1, 1);
  h.Put(2, 2);

  ASSERT_EQ(h.Get(1), std::make_optional(1));
  ASSERT_EQ(h.Get(2), std::make_optional(2));
  ASSERT_EQ(h.Get(3), std::nullopt);
}

auto main(int argc, char **argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::FLAGS_gtest_output = "stdout:";
  return RUN_ALL_TESTS();
}
