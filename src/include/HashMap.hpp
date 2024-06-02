#pragma once

#include <fmt/format.h>

#include <cstddef>
#include <limits>
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
  /**
   *The default initial capacity - MUST be a power of two.
   */
  constexpr static std::size_t DEFAULT_INITIAL_CAPACITY = 1 << 4;  // aka 16

  /**
   *The maximum capacity, used if a higher value is implicitly specified by
   *either of the constructors with arguments. MUST be a power of two <= 1<<30.
   */
  constexpr static std::size_t MAXIMUM_CAPACITY = 1 << 30;

  constexpr static std::hash<K> HASH_FUNCTION;

  constexpr static std::size_t HASHCODE_REMOVE_SIZE = 1 << 4;

  /**
   * The load factor used when none specified in constructor.
   */
  constexpr static float DEFAULT_LOAD_FACTOR = 0.75;

  static inline auto hash(const K &key) noexcept -> std::size_t {
    std::size_t h = HASH_FUNCTION(key);
    return h ^ (h >> HASHCODE_REMOVE_SIZE);
  }

  struct Node {
    // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    const std::size_t hash_code;

    // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    const K key;

    // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    V value;

    // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
    Node *next;

    Node(std::size_t hash_code, K key, V value, Node *next)
        : hash_code(hash_code), key(key), value(value), next(next) {}

    [[nodiscard]] auto toString() const noexcept -> std::string {
      // type check
      static_assert(fmt::is_formattable<K>::value, "K must impl fmt");
      static_assert(fmt::is_formattable<V>::value, "V must impl fmt");
      return fmt::format("{} = {}", key, value);
    }
  };

  std::size_t capacity_;

  std::size_t threshold_;

  std::vector<Node *> tables_;

  std::size_t size_ = 0;

  auto resize() noexcept -> void{
      // std::size_t oldCap = tables_.size();
      // std::size_t oldThr = threshold_;
      // std::size_t newCap = 0;
      // std::size_t newThr = 0;

      // if (oldCap >= MAXIMUM_CAPACITY) {
      //   threshold_ = std::numeric_limits<std::size_t>::max();
      //   return;
      // }

      // if ((newCap = (oldCap << 1)) < MAXIMUM_CAPACITY &&
      //     oldCap >= DEFAULT_INITIAL_CAPACITY) {
      //   newThr = oldThr << 1;
      // }
  };

 public:
  HashMap() noexcept
      : capacity_(DEFAULT_INITIAL_CAPACITY),
        tables_(std::vector<Node *>(DEFAULT_INITIAL_CAPACITY, nullptr)),
        threshold_(capacity_ * DEFAULT_LOAD_FACTOR){};

  template <typename KeyType, typename ValueType>
  auto Put(KeyType &&key, ValueType &&value) noexcept -> void {
    // type check
    using DecayedKeyType = std::decay_t<KeyType>;
    using DecayedValueType = std::decay_t<ValueType>;
    static_assert(std::is_same_v<DecayedKeyType, K>,
                  "KeyType must be the same as K");
    static_assert(std::is_same_v<DecayedValueType, V>,
                  "ValueType must be the same as V");

    std::size_t hashCode = hash(key);
    std::size_t index = hashCode & (capacity_ - 1);

    if (tables_[index] == nullptr) {
      tables_[index] = new Node(hashCode, key, value, nullptr);

      if (++size_ > threshold_) {
        resize();
      }

      return;
    }

    // tables_[index] != nullptr
    Node *node = tables_[index];
    while (node->next != nullptr) {
      if (node->hash_code == hashCode && node->key == key) {
        node->value = value;
        break;
      }
      node = node->next;
    }

    // node -> next
    if (node->next == nullptr) {
      if (node->hash_code == hashCode && node->key == key) {
        node->value = value;
      } else {
        node->next = new Node(hashCode, key, value, nullptr);

        if (++size_ > threshold_) {
          resize();
        }
      }
    }
  };

  auto Get(const K &key) const noexcept -> std::optional<V> {
    std::size_t hashCode = hash(key);
    Node *node = tables_[hash(key) & (capacity_ - 1)];

    while (node != nullptr) {
      if (node->hash_code == hashCode && node->key == key) {
        return std::make_optional(node->value);
      }
      node = node->next;
    }

    return std::nullopt;
  }

  auto Contain(const K &key) const noexcept -> bool {
    std::size_t hashCode = hash(key);
    Node *node = tables_[hash(key) & (capacity_ - 1)];

    while (node != nullptr) {
      if (node->hash_code == hashCode && node->key == key) {
        return true;
      }
      node = node->next;
    }

    return false;
  }

  [[nodiscard]] auto toString() const noexcept -> std::string {
    // type check
    static_assert(fmt::is_formattable<K>::value, "K must impl fmt");
    static_assert(fmt::is_formattable<V>::value, "V must impl fmt");

    std::string ans("{");
    for (const Node *node : tables_) {
      while (node != nullptr) {
        ans.append(node->toString());
        ans.append(",");
        node = node->next;
      }
    }

    if (ans.length() != 1) {
      ans[ans.length() - 1] = '}';
    } else {
      ans.append("}");
    }

    return ans;
  }

  [[nodiscard]] auto size() const noexcept -> std::size_t { return size_; }

  [[nodiscard]] auto empty() const noexcept -> bool { return size_ == 0; }
};

}  // namespace JAVA

template <typename K, typename V>
struct fmt::formatter<JAVA::HashMap<K, V>> {
  constexpr auto parse(fmt::format_parse_context &ctx)
      -> decltype(ctx.begin()) {
    return ctx.end();
  }

  template <typename FormatContext>
  auto format(const JAVA::HashMap<K, V> &myType, FormatContext &ctx) const
      -> decltype(ctx.out()) {
    return fmt::format_to(ctx.out(), "{}", myType.toString());
  }
};