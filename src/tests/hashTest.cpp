#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <utility>

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
TEST(hashTestBasicType, AssertionTrue) {
  std::hash<int> int_hash;
  ASSERT_EQ(int_hash(42), int_hash(42));
  ASSERT_NE(int_hash(10), int_hash(42));

  std::hash<std::string> string_hash;
  ASSERT_EQ(string_hash("Hello world"), string_hash("Hello world"));
  ASSERT_NE(string_hash("Hello world"), string_hash("Hello world1"));
}

class MyClass {
 private:
  friend class std::hash<MyClass>;

  std::string name;
  std::size_t age;

 public:
  MyClass(std::string name, std::size_t age) noexcept
      : name(std::move(name)), age(age) {}

  auto operator==(const MyClass &other) const noexcept -> bool {
    return name == other.name && age == other.age;
  }
};

template <>
struct std::hash<MyClass> {
  auto operator()(const MyClass &obj) const -> std::size_t {
    __uint128_t ans = 0;
    ans += std::hash<std::size_t>()(obj.age);
    ans %= std::numeric_limits<std::size_t>::max();
    ans += std::hash<std::string>()(obj.name);
    ans %= std::numeric_limits<std::size_t>::max();
    return static_cast<std::size_t>(ans &
                                    std::numeric_limits<std::size_t>::max());
  }
};

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
TEST(hashTestMyClassType, AssertionTrue) {
  // NOLINTNEXTLINE(readability-magic-numbers)
  MyClass obj1("jack", 42);
  // NOLINTNEXTLINE(readability-magic-numbers)
  MyClass obj2("Tom", 42);
  // NOLINTNEXTLINE(readability-magic-numbers)
  MyClass obj3("Tom", 42);
  std::hash<MyClass> MyClass_hash;
  ASSERT_NE(MyClass_hash(obj1), MyClass_hash(obj2));
  ASSERT_EQ(MyClass_hash(obj3), MyClass_hash(obj2));
}

auto main(int argc, char **argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::FLAGS_gtest_output = "stdout:";
  return RUN_ALL_TESTS();
}
