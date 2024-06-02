#pragma once

#include <fmt/format.h>

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>
namespace JAVA {
template <typename K, typename V>
class HashMap {
 private:
  constexpr static std::size_t DEFAULT_INITIAL_CAPACITY = 1 << 4;  // aka 16

  constexpr static std::size_t MAXIMUM_CAPACITY = 1 << 30;

  struct Node {
    // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    const std::size_t hash;

    // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    const K key;

    // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    V value;

    // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    Node *next;

    Node(std::size_t hash, K key, V value, Node *next)
        : hash(hash), key(key), value(value), next(next) {}

    [[nodiscard]] auto toString() const noexcept -> std::string {
      return fmt::format("{} = {}", key, value);
    }
  };

 public:
  HashMap() noexcept = default;

  template <typename KeyType, typename ValueType>
  auto Put(KeyType &&key, ValueType &&value) noexcept -> void {
    using DecayedKeyType = std::decay_t<KeyType>;
    using DecayedValueType = std::decay_t<ValueType>;

    static_assert(std::is_same_v<DecayedKeyType, K>,
                  "KeyType must be the same as K");
    static_assert(std::is_same_v<DecayedValueType, V>,
                  "ValueType must be the same as V");
    fmt::println("key = {}, value = {}", key, value);
  };

  auto Get(const K &key) noexcept(false) -> std::optional<V> {
    throw std::logic_error("not impl");
  }
};
}  // namespace JAVA