#include <bitset>
#include <iostream>
#include <random>
#include <type_traits>

typedef uint64_t my_key_type;

void check_type() {
  if (std::is_same<my_key_type, uint64_t>::value) {
    std::cout << "my_key_type is uint64_t\n";
  } else if (std::is_same<my_key_type, __uint128_t>::value) {
    std::cout << "my_key_type is __uint128_t\n";
  } else {
    throw std::runtime_error("my_key_type is of an unknown type!");
  }
}

u_int64_t generate_pseudorandom_uint64(std::mt19937_64 &gen_64) {
  return gen_64();
}

__uint128_t generate_pseudorandom_uint128(std::mt19937_64 &gen_64) {
  u_int64_t high_bits = gen_64();
  u_int64_t low_bits = gen_64();

  __uint128_t all_bits = ((__uint128_t)high_bits << 64) | low_bits;

  return all_bits;
}

void dispalay_bits_uint64(uint64_t some_int) {
    std::cout << "All bits = " << std::bitset<64>(some_int) << std::endl;
}

void display_bits_uint128(__uint128_t some_int) {
    uint64_t low_bits = static_cast<uint64_t>(some_int);
    uint64_t high_bits = static_cast<uint64_t>(some_int >> 64);
    std::cout << "Hi bits = " << std::bitset<64>(high_bits) << std::endl;
    std::cout << "Lo bits = " << std::bitset<64>(low_bits) << std::endl;
}

void display_bits(my_key_type my_key) {
    if (std::is_same<my_key_type, uint64_t>::value) {
        dispalay_bits_uint64(my_key);
    } else if (std::is_same<my_key_type, __uint128_t>::value) {
        display_bits_uint128(my_key);
    } else {
        throw std::runtime_error("my_key_type is not supported.");
    }
}

my_key_type generate_pseusorandom_key(std::mt19937_64 &gen_64) {
    my_key_type result{};
    if (std::is_same<my_key_type, uint64_t>::value) {
        result = generate_pseudorandom_uint64(gen_64);
    } else if (std::is_same<my_key_type, __uint128_t>::value) {
        result = generate_pseudorandom_uint128(gen_64);
    } else {
        throw std::runtime_error("my_key_type is not supported.");
    }
    return result;
}


u_int16_t bit_concat_practice(u_int8_t low_bits, u_int8_t high_bits) {
  u_int16_t all_bits = ((u_int16_t)high_bits << 8) | low_bits;

  std::cout << "my 8 high bits = " << std::bitset<8>(high_bits) << std::endl;
  std::cout << "my 8 low bits = " << std::bitset<8>(low_bits) << std::endl;
  std::cout << "my 16 combined bits = " << std::bitset<16>(all_bits)
            << std::endl;

  return all_bits;
}

int main() {
//   // practice & check bit concatenation
//   u_int8_t my_8_low_bits{4};
//   u_int8_t my_8_high_bits{16};
//   auto my_16_combined_bits =
//       bit_concat_practice(my_8_low_bits, my_8_high_bits);

//   // practice checking size of my_key_type
//   auto key_size_in_bytes = num_bytes_in_key();
//   std::cout << "Size of my_key type is " << key_size_in_bytes << std::endl;

  //   use our custom check_type function
  try {
    check_type();

  } catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

int seed{123456};
std::mt19937_64 gen_64(seed);
auto pseudorandom_key = generate_pseusorandom_key(gen_64);

display_bits(pseudorandom_key);

}
