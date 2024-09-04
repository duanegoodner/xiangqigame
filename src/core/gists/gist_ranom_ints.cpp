#include <iostream>
#include <random>

template <typename T>
T random_in_range(T low, T high) {
  std::random_device rd;
  std::mt19937 engine(rd());
  std::uniform_int_distribution<T> dist(low, high);
  return dist(engine);
}

struct Key128 {
  uint64_t high;
  uint64_t low;

  bool operator==(const Key128 &other) const {
    return high == other.high && low == other.low;
  }

  // Generates pseudorandom Key128 using an existing 64-bit generator
  static const Key128 pseudorandom_from_existing_64bit_gen(
      std::mt19937_64 &gen_64
  ) {
    return Key128{gen_64(), gen_64()};
  }
  // Creates new 64-bit generator w/ truly random 32-bit seed, then uses this
  // generator to create pseudorandom Key128.
  static const Key128 pseudo_random_from_new_64bit_gen() {
    std::random_device rd{};
    uint32_t seed = rd();
    std::mt19937_64 gen_64{seed};
    return pseudorandom_from_existing_64bit_gen(gen_64);
  }

  void print() {
    std::cout << high << std::endl;
    std::cout << low << std::endl;
  }

  Key128 operator^(const Key128 &other) const {
    return Key128{high ^ other.high, low ^ other.low};
  }
};

// Key128 operator^(const Key128 &lhs, Key128 &rhs) {
//   return Key128{lhs.high ^ rhs.high, lhs.low ^ rhs.low};
// }

int main() {

  std::random_device local_rd;
  std::mt19937_64 gen_64(local_rd());

  auto local_128_a = Key128::pseudorandom_from_existing_64bit_gen(gen_64);
  auto local_128_b = Key128::pseudorandom_from_existing_64bit_gen(gen_64);
  auto local_128_c = Key128::pseudorandom_from_existing_64bit_gen(gen_64);

  std::cout << "local_128_a" << std::endl;
  local_128_a.print();
  std::cout << std::endl;

  std::cout << "local_128_b" << std::endl;
  local_128_b.print();
  std::cout << std::endl;

  std::cout << "local_128_c" << std::endl;
  local_128_c.print();
  std::cout << std::endl;

  auto local_128_d = local_128_a ^ local_128_b;
  std::cout << "local_128_d" << std::endl;
  local_128_d.print();
  std::cout << std::endl;

  auto local_128_undo_xor = local_128_d ^ local_128_b;
  std::cout << "local_128_undo_xor" << std::endl;
  local_128_undo_xor.print();
  std::cout << std::endl;

  auto local_128_g = Key128::pseudo_random_from_new_64bit_gen();
  auto local_128_h = Key128::pseudo_random_from_new_64bit_gen();
  auto local_128_i = Key128::pseudo_random_from_new_64bit_gen();

  local_128_g.print();
  std::cout << std::endl;
  local_128_h.print();
  std::cout << std::endl;
  local_128_i.print();
  std::cout << std::endl;

  return 0;
}