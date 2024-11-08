#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>

template <typename IntType>
IntType generate_random_int() {
  std::random_device rd;
  IntType result = 0;

  auto bits_per_block = 8 * sizeof(std::random_device::result_type);
  auto num_blocks = sizeof(IntType) / sizeof(std::random_device::result_type);
  for (auto idx = 0; idx < num_blocks; ++idx) {
    result <<= bits_per_block;
    result |= static_cast<IntType>(rd());
  }
  return result;
}

template <typename IntType>
void print_hex(IntType num) {
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

int main() {

  uint32_t random_032 = generate_random_int<uint32_t>();
  print_hex(random_032);

  uint64_t random_064 = generate_random_int<uint64_t>();
  print_hex(random_064);

  __uint128_t random_128 = generate_random_int<__uint128_t>();
  print_hex(random_128);

  return 0;
}
