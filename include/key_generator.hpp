//! @file key_generator.hpp
//! Declaration of boardstate::KeyGenerator and implementation of its template methods.

#pragma once

#include <bitset>
#include <iostream>
#include <random>
#include <type_traits>

namespace boardstate {

template <typename IntType>
class PseudoRandomKeyGenerator {
public:
  PseudoRandomKeyGenerator()
      : seed_{std::random_device{}()}
      , prng_{seed_} {}

  PseudoRandomKeyGenerator(uint32_t seed)
      : seed_{seed}
      , prng_{seed_} {}

  //! Generates a pseudorandom IntType value using mt19937.
  //! The 0xFFFFFFFF bit mask is used b/c std::mt19937 uses std::uint_fast32_t which is a
  //! 64-bit integer on 64-bit systems (but still only uses the lower 32 bits).
  //! See discussion at:
  //! https://stackoverflow.com/questions/63601328/why-do-std-implementations-of-mt19937-have-double-sizeof-as-boost-version
  IntType GenerateKey() {
    IntType result = 0;
    auto bits_per_block = 8 * sizeof(uint32_t);
    auto blocks_per_int = sizeof(IntType) / sizeof(uint32_t);
    for (auto idx = 0; idx < blocks_per_int; ++idx) {
      result <<= bits_per_block;
      result |= static_cast<IntType>(prng_() & 0xFFFFFFFF);
    }
    return result;
  }

  std::random_device::result_type seed() { return seed_; }

private:
  std::random_device::result_type seed_;
  std::mt19937 prng_;
};

//! Template for class that generates random integer values of IntType.
//! Uses std::random_device to build ints in 32-bit blocks.
template <typename IntType>
class RandomKeyGenerator {
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

} // namespace boardstate
