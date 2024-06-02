#pragma once

#include <fmt/format.h>

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace JAVA {

template <typename K, typename V>
class HashMap {
  // NOLINTNEXTLINE(readability-magic-numbers)
  static_assert(sizeof(std::size_t) == 8);

  static_assert(
      std::is_same_v<decltype(std::declval<K>() == std::declval<K>()), bool>,
      "type K must impl operator==");

 private:
  constexpr static std::size_t DEFAULT_INITIAL_CAPACITY = 1 << 4;  // aka 16

  constexpr static std::size_t MAXIMUM_CAPACITY = 1 << 30;

  constexpr static std::hash<K> HASH_FUNCTION;

  constexpr static std::size_t REMOVE_SIZE = 1 << 4;

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

  std::size_t size_;

  std::size_t capacity_;

  std::vector<Node *> tables_;

  static auto hash(const K &key) noexcept -> std::size_t {
    std::size_t h = HASH_FUNCTION(key);
    return h ^ (h >> REMOVE_SIZE);
  }

 public:
  HashMap() noexcept
      : capacity_(DEFAULT_INITIAL_CAPACITY),
        tables_(std::vector<Node *>(DEFAULT_INITIAL_CAPACITY, nullptr)){};

  template <typename KeyType, typename ValueType>
  auto Put(KeyType &&key, ValueType &&value) noexcept -> void {
    using DecayedKeyType = std::decay_t<KeyType>;
    using DecayedValueType = std::decay_t<ValueType>;
    static_assert(std::is_same_v<DecayedKeyType, K>,
                  "KeyType must be the same as K");
    static_assert(std::is_same_v<DecayedValueType, V>,
                  "ValueType must be the same as V");

    fmt::println("key = {}, value = {}", key, value);

    std::size_t hashCode = hash(std::forward<KeyType>(key));
    std::size_t index = hashCode & (capacity_ - 1);
    if (tables_[index] == nullptr) {
      tables_[index] = new Node(hashCode, key, value, nullptr);
    } else {
      Node *node = tables_[index];
      while (node->next != nullptr) {
        if (node->hash == hashCode && node->key == key) {
          node->value = value;
          break;
        }
        node = node->next;
      }

      // node -> next
      if (node->next == nullptr) {
        if (node->hash == hashCode && node->key == key) {
          node->value = value;
        } else {
          node->next = new Node(hashCode, key, value, nullptr);
        }
      }
    }
  };

  auto Get(const K &key) noexcept(false) -> std::optional<V> {
    std::size_t hashCode = hash(key);
    std::size_t index = hashCode & (capacity_ - 1);

    Node *node = tables_[index];

    while (node != nullptr) {
      if (node->hash == hashCode && node->key == key) {
        return std::make_optional(node->value);
      }
    }

    return std::nullopt;
  }

  [[nodiscard]] auto size() const noexcept -> std::size_t { return size_; }
};

}  // namespace JAVA