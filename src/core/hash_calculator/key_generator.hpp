#pragma once

#include <bitset>
#include <common.hpp>
#include <iostream>
#include <random>
#include <type_traits>

// typedef uint64_t zkey_t;

namespace KeyGenerator {
template <typename T>
T generate_key(std::mt19937_64 &gen_64) {
  static_assert(sizeof(T) == 0, "Type  not supported.");
  T dummy_result{};
  return dummy_result;
}

template <typename T>
void display_key(T key) {
   static_assert(sizeof(T) == 0, "Type  not supported."); 
}

template <>
inline uint64_t generate_key(std::mt19937_64 &gen_64) {
  return gen_64();
}

template <>
inline void display_key(uint64_t key) {
    std::cout << "all bits = " << std::bitset<64>(key) << std::endl;
}

template <>
inline __uint128_t generate_key(std::mt19937_64 &gen_64) {
  uint64_t high_bits = gen_64();
  uint64_t low_bits = gen_64();

  __uint128_t all_bits = ((__uint128_t)high_bits << 64) | low_bits;

  return all_bits;
}

template <>
inline void display_key(__uint128_t key) {
    uint64_t hi_bits = static_cast<uint64_t>(key >> 64);
    uint64_t lo_bits = static_cast<uint64_t>(key);    
    std::cout << "hi bits = " << std::bitset<64>(hi_bits) << std::endl;
    std::cout << "lo bits = " << std::bitset<64>(lo_bits) << std::endl;
}

inline zkey_t generate_zkey(std::mt19937_64 &gen_64) {
  return generate_key<zkey_t>(gen_64);
}

inline void display_zkey(zkey_t key) {
    display_key<zkey_t>(key);
}
} // namespace KeyGenerator

