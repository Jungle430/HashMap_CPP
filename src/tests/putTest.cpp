#include <fmt/format.h>
#include <gtest/gtest.h>

#include <optional>
#include <string>

#include "../include/HashMap.hpp"

// NOLINTNEXTLINE(modernize-use-trailing-return-type,readability-function-cognitive-complexity)
TEST(PutTestBasicType, AssertionTrue) {
  JAVA::HashMap<int, int> h1;
  // NOLINTNEXTLINE(readability-magic-numbers)
  for (int i = 0; i < 1000; i++) {
    h1.Put(i, i + 1);
  }
  // NOLINTNEXTLINE(readability-magic-numbers)
  for (int i = 0; i < 10000; i++) {
    // NOLINTNEXTLINE(readability-magic-numbers)
    if (i < 1000) {
      ASSERT_EQ(h1.Get(i), std::make_optional(i + 1));
    } else {
      ASSERT_EQ(h1.Get(i), std::nullopt);
    }
  }

  JAVA::HashMap<const char *, int> h2;
  h2.Put("Hello world", 1);
  h2.Put("Hello world", 2);
  h2.Put("Hello world1", 3);

  ASSERT_NE(h2.Get("Hello world"), std::make_optional(1));
  ASSERT_EQ(h2.Get("Hello world"), std::make_optional(2));
  ASSERT_EQ(h2.Get("Hello world1"), std::make_optional(3));
  ASSERT_EQ(h2.Get("Hello world2"), std::nullopt);
}

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
TEST(PutTestString, AssertionTrue) {
  JAVA::HashMap<std::string, int> h3;
  h3.Put(std::string("Hello world"), 1);
  h3.Put(std::string("Hello world"), 2);
  h3.Put(std::string("Hello world1"), 3);

  ASSERT_NE(h3.Get(std::string("Hello world")), std::make_optional(1));
  ASSERT_EQ(h3.Get(std::string("Hello world")), std::make_optional(2));
  ASSERT_EQ(h3.Get(std::string("Hello world1")), std::make_optional(3));
  ASSERT_EQ(h3.Get(std::string("Hello world2")), std::nullopt);
}

//======================================================================

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
    std::size_t ans = 0;
    ans += std::hash<std::size_t>()(obj.age);
    ans += std::hash<std::string>()(obj.name);
    return ans;
  }
};

// NOLINTNEXTLINE(modernize-use-trailing-return-type)
TEST(PutTestMyClassType, AssertionTrue) {
  // NOLINTNEXTLINE(readability-magic-numbers)
  MyClass obj1("jack", 42);
  // NOLINTNEXTLINE(readability-magic-numbers)
  MyClass obj2("Tom", 42);
  // NOLINTNEXTLINE(readability-magic-numbers)
  MyClass obj3("Tom", 42);
  // NOLINTNEXTLINE(readability-magic-numbers)
  MyClass obj4("Tom", 45);

  ASSERT_EQ(obj2, obj3);
  JAVA::HashMap<MyClass, std::string> h;
  h.Put(obj1, std::string("lisa"));
  ASSERT_EQ(h.Get(obj1), std::make_optional(std::string("lisa")));

  h.Put(obj1, std::string("liSA"));
  ASSERT_EQ(h.Get(obj1), std::make_optional(std::string("liSA")));

  h.Put(obj2, std::string("Kali"));
  ASSERT_EQ(h.Get(obj2), std::make_optional(std::string("Kali")));
  ASSERT_EQ(h.Get(obj2), h.Get(obj3));

  ASSERT_EQ(h.Get(obj4), std::nullopt);
}

auto main(int argc, char **argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::FLAGS_gtest_output = "stdout:";
  return RUN_ALL_TESTS();
}
