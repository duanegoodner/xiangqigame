//! @file key_generator.hpp
//! Declaration of KeyGenerator and implementation of its template methods.

#pragma once

#include <bitset>
#include <iostream>
#include <random>
#include <type_traits>

namespace boardstate {

class KeyGenerator {
public:
  KeyGenerator(unsigned long seed = std::random_device{}())
      : gen_64_{seed} {}

  template <typename T>
  T GenerateKey();

  template <typename T>
  void DisplayKey(T key);

private:
  std::mt19937_64 gen_64_;
};

template <>
inline uint64_t KeyGenerator::GenerateKey<uint64_t>() {
  return gen_64_();
}

template <>
inline void KeyGenerator::DisplayKey(uint64_t key) {
  std::cout << "all bits = " << std::bitset<64>(key) << std::endl;
}

template <>
inline __uint128_t KeyGenerator::GenerateKey<__uint128_t>() {
  uint64_t high_bits = gen_64_();
  uint64_t low_bits = gen_64_();
  __uint128_t all_bits = ((__uint128_t)high_bits << 64) | low_bits;
  return all_bits;
}

template <>
inline void KeyGenerator::DisplayKey(__uint128_t key) {
  uint64_t hi_bits = static_cast<uint64_t>(key >> 64);
  uint64_t lo_bits = static_cast<uint64_t>(key);
  std::cout << "hi bits = " << std::bitset<64>(hi_bits) << std::endl;
  std::cout << "lo bits = " << std::bitset<64>(lo_bits) << std::endl;
}

} // namespace boardstate
