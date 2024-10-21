//! @file key_generator.hpp
//! Declaration of boardstate::KeyGenerator and implementation of its template methods.

#pragma once

#include <bitset>
#include <iostream>
#include <random>
#include <type_traits>

namespace boardstate {

template <typename IntType>
class KeyGeneratorNew {
public:
  IntType GenerateKey() {
    IntType result = 0;
    auto bits_per_block = 8 * sizeof(std::random_device::result_type);
    auto blocks_per_int = sizeof(IntType) / sizeof(std::random_device::result_type);
    for (auto idx = 0; idx < blocks_per_int; ++idx) {
      result <<= bits_per_block;
      result |= static_cast<IntType>(rd_());
    }
    return result;
  }

private:
  std::random_device rd_;
};

template <typename IntType>
void PrintHex(IntType num) {
  constexpr int bits_per_block = 8 * sizeof(std::random_device::result_type);
  constexpr int bits_per_hex_digit = 4; // Each hex digit represents 4 bits
  constexpr int hex_digits_per_block =
      bits_per_block / bits_per_hex_digit; // Hex digits per block
  constexpr int total_blocks =
      (sizeof(IntType) * 8 + bits_per_block - 1) /
      bits_per_block; // Calculate the number of 32-bit blocks needed

  std::cout << "0x";
  for (int i = total_blocks - 1; i >= 0; --i) {
    // Extract each 32-bit block
    uint32_t block = static_cast<uint32_t>(num >> (i * bits_per_block));
    // Print the block, handling leading zeros within each block
    std::cout << std::hex << std::setfill('0') << std::setw(hex_digits_per_block)
              << block;
  }
  std::cout << std::dec << std::endl; // Reset to decimal output
}

//! Used by boardstate::ZobristKeys to generate hash keys. Supports 64-bit and 128-bit
//! keys.
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
