#pragma once

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <limits>
#include <optional>
#include <sstream>
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

  /**
   * Hash Function, if is not the basic type, you can using your hash function
   * by Instantiated template
   */
  constexpr static std::hash<K> HASH_FUNCTION = std::hash<K>();

  constexpr static std::size_t HASHCODE_REMOVE_SIZE = 1 << 4;

  /**
   * The load factor used when none specified in constructor.
   */
  constexpr static float DEFAULT_LOAD_FACTOR = 0.75;

  static inline auto hash(const K &key) noexcept -> std::size_t {
    std::size_t h = HASH_FUNCTION(key);
    return h ^ (h >> HASHCODE_REMOVE_SIZE);
  }

  template <typename T>
  struct is_streamable {
    template <typename U>
    static auto test(int)
        -> decltype(std::declval<std::ostream &>() << std::declval<U>(),
                    std::true_type{});

    template <typename>
    static auto test(...) -> std::false_type;

    static constexpr bool value = decltype(test<T>(0))::value;
  };

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
      static_assert(is_streamable<K>::value, "K must impl operator << ");
      static_assert(is_streamable<V>::value, "V must impl operator << ");
      std::stringstream ss;
      ss << key << "=" << value;
      return ss.str();
    }
  };

  class ObjectPool {
   private:
    constexpr static std::size_t DEFAULT_BLOCK_SIZE = 1 << 10;

    constexpr static float DEFAULT_FREE_DEALLOCATE_NODE_LOAD_FACTOR = 1.75;

    std::vector<Node *> free_list_;

    std::vector<Node *> del_list;

    std::size_t blockSize_;

    std::size_t size_ = 0;

    std::size_t index_ = 0;

    float free_deallocate_node_load_factor_;

    auto allocateBlock() noexcept -> void {
      for (std::size_t i = 0; i < blockSize_; ++i) {
        free_list_[i] = (static_cast<Node *>(::operator new(sizeof(Node))));
      }
      size_ += blockSize_;
      index_ = 0;
    }

    auto freeBlock() noexcept -> void {
      std::size_t old_size = del_list.size();
      for (Node *node : del_list) {
        ::operator delete(node);
      }
      del_list.clear();
      size_ -= old_size;
    }

   public:
    explicit ObjectPool(size_t blockSize = DEFAULT_BLOCK_SIZE,
                        float free_deallocate_node_load_factor =
                            DEFAULT_FREE_DEALLOCATE_NODE_LOAD_FACTOR)
        : blockSize_(blockSize),
          free_list_(std::vector<Node *>(blockSize, nullptr)),
          free_deallocate_node_load_factor_(free_deallocate_node_load_factor) {
      allocateBlock();
    }

    auto allocate() noexcept -> Node * {
      if (index_ == blockSize_) {
        allocateBlock();
      }
      size_--;
      return free_list_[index_++];
    }

    auto deallocate(Node *node) -> void {
      node->~Node();
      std::memset(node, 0, sizeof(Node));
      del_list.emplace_back(node);
      if (++size_ > blockSize_ * free_deallocate_node_load_factor_) {
        freeBlock();
      }
    }

    ~ObjectPool() {
      while (index_ < blockSize_) {
        ::operator delete(free_list_[index_++]);
      }

      freeBlock();
    }
  };

  ObjectPool objectPool_;

  std::size_t capacity_;

  std::size_t threshold_;

  std::vector<Node *> tables_;

  std::size_t size_ = 0;

  float loadFactor;

  // NOLINTNEXTLINE(readability-function-cognitive-complexity)
  auto resize() noexcept -> void {
    std::size_t oldCap = tables_.size();
    std::size_t oldThr = threshold_;
    std::size_t newCap = 0;
    std::size_t newThr = 0;

    if (oldCap >= MAXIMUM_CAPACITY) {
      threshold_ = std::numeric_limits<std::size_t>::max();
      return;
    }

    if ((newCap = (oldCap << 1)) < MAXIMUM_CAPACITY &&
        oldCap >= DEFAULT_INITIAL_CAPACITY) {
      newThr = oldThr << 1;
    }

    if (newThr == 0) {
      float ft = static_cast<float>(newCap) * loadFactor;
      newThr = (newCap < MAXIMUM_CAPACITY &&
                        ft < static_cast<float>(MAXIMUM_CAPACITY)
                    ? static_cast<std::size_t>(ft)
                    : std::numeric_limits<std::size_t>::max());
    }

    capacity_ = newCap;
    threshold_ = newThr;
    std::vector<Node *> newTable(newCap, nullptr);

    // move data
    for (std::size_t i = 0; i < oldCap; i++) {
      Node *e = nullptr;

      if ((e = tables_[i]) != nullptr) {
        // table -> e -> nullptr
        if (e->next == nullptr) {
          newTable[e->hash_code & (newCap - 1)] = e;
        } else {
          // table -> e1 -> e2 -> ...
          Node *loHead = nullptr;
          Node *loTail = nullptr;

          Node *hiHead = nullptr;
          Node *hiTail = nullptr;

          Node *next = nullptr;

          do {
            next = e->next;

            if ((e->hash_code & oldCap) == 0) {
              // no need to move(because the bit length <= oldCap)
              if (loTail == nullptr) {
                loHead = e;
              } else {
                loTail->next = e;
              }
              loTail = e;
            } else {
              // need to move
              if (hiTail == nullptr) {
                hiHead = e;
              } else {
                hiTail->next = e;
              }
              hiTail = e;
            }

          } while ((e = next) != nullptr);

          if (loTail != nullptr) {
            loTail->next = nullptr;
            newTable[i] = loHead;
          }

          if (hiTail != nullptr) {
            hiTail->next = nullptr;
            newTable[i + oldCap] = hiHead;
          }
        }
      }
    }
    tables_ = std::move(newTable);
  };

 public:
  HashMap() noexcept
      : capacity_(DEFAULT_INITIAL_CAPACITY),
        tables_(std::vector<Node *>(DEFAULT_INITIAL_CAPACITY, nullptr)),
        threshold_(capacity_ * DEFAULT_LOAD_FACTOR),
        loadFactor(DEFAULT_LOAD_FACTOR),
        objectPool_(){};

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
      tables_[index] =
          new (objectPool_.allocate()) Node(hashCode, key, value, nullptr);

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
        node->next =
            new (objectPool_.allocate()) Node(hashCode, key, value, nullptr);

        if (++size_ > threshold_) {
          resize();
        }
      }
    }
  };

  auto Get(const K &key) const noexcept -> std::optional<V> {
    std::size_t hashCode = hash(key);
    Node *node = tables_[hashCode & (capacity_ - 1)];

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
    Node *node = tables_[hashCode & (capacity_ - 1)];

    while (node != nullptr) {
      if (node->hash_code == hashCode && node->key == key) {
        return true;
      }
      node = node->next;
    }

    return false;
  }

  auto Del(const K &key) noexcept -> bool {
    std::size_t hashCode = hash(key);
    Node *prev = nullptr;
    Node *curr = tables_[hashCode & (capacity_ - 1)];
    while (curr != nullptr) {
      Node *next = curr->next;
      if (curr->hash_code == hashCode && curr->key == key) {
        // curr = tables_[hashCode & (capacity_ - 1)];
        if (prev == nullptr) {
          tables_[hashCode & (capacity_ - 1)] = next;
        } else {
          // prev -> curr -> next
          prev->next = next;
        }
        objectPool_.deallocate(curr);
        size_--;
        return true;
      }
      prev = curr;
      curr = next;
    }
    return false;
  }

  [[nodiscard]] auto toString() const noexcept -> std::string {
    // type check
    static_assert(is_streamable<K>::value, "K must impl operator << ");
    static_assert(is_streamable<V>::value, "V must impl operator << ");

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

  ~HashMap() {
    for (Node *tableNode : tables_) {
      Node *curr = tableNode;
      Node *next = tableNode;
      while (curr != nullptr) {
        next = curr->next;
        objectPool_.deallocate(curr);
        curr = next;
      }
    }
  }
};

}  // namespace JAVA
