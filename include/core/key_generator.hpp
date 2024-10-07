// Filename: game_board.tpp
// Author: Duane Goodner
// Created: 2024-09-03
// Last Modified: 2024-09-04

// Description:
// Header only library for creating and displaying ints used as Zobrist keys.

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

// namespace keygenerator {

// // Generic template for key generation. Supported int types will have
// // specialization. If control flow calls generic, we are NOT using supported
// // type, and static_assert will cause compiler error.
// template <typename T>
// T generate_key(std::mt19937_64 &gen_64) {
//   static_assert(sizeof(T) == 0, "Type  not supported.");
//   T dummy_result{};
//   return dummy_result;
// }

// // Generic template for displaying a key.
// template <typename T>
// void display_key(T key) {
//   static_assert(sizeof(T) == 0, "Type  not supported.");
// }

// // Specialization for generating 64 bit key
// template <>
// inline uint64_t generate_key(std::mt19937_64 &gen_64) {
//   return gen_64();
// }

// // Specialization for displaying 64 bit key
// template <>
// inline void display_key(uint64_t key) {
//   std::cout << "all bits = " << std::bitset<64>(key) << std::endl;
// }

// // Specialization for generating 128 bit key
// template <>
// inline __uint128_t generate_key(std::mt19937_64 &gen_64) {
//   uint64_t high_bits = gen_64();
//   uint64_t low_bits = gen_64();

//   __uint128_t all_bits = ((__uint128_t)high_bits << 64) | low_bits;

//   return all_bits;
// }

// // Specialization for generating 64 bit key
// template <>
// inline void display_key(__uint128_t key) {
//   uint64_t hi_bits = static_cast<uint64_t>(key >> 64);
//   uint64_t lo_bits = static_cast<uint64_t>(key);
//   std::cout << "hi bits = " << std::bitset<64>(hi_bits) << std::endl;
//   std::cout << "lo bits = " << std::bitset<64>(lo_bits) << std::endl;
// }

// } // namespace keygenerator
