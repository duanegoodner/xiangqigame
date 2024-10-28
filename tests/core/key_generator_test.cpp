// #include <common.hpp>
#include <gtest/gtest.h>
#include <key_generator.hpp>
#include <random>

class PseudoRandomKeyGeneratorTest : public ::testing::Test {
protected:
};

TEST_F(PseudoRandomKeyGeneratorTest, DefaultConstructor) {
  boardstate::PseudoRandomKeyGenerator<uint32_t> key_generator_032{};
  boardstate::PseudoRandomKeyGenerator<uint64_t> key_generator_064{};
  boardstate::PseudoRandomKeyGenerator<__uint128_t> key_generator_128{};
}

TEST_F(PseudoRandomKeyGeneratorTest, ConstructorWithSeed) {
  boardstate::PseudoRandomKeyGenerator<uint32_t> key_generator_032{1234};
  boardstate::PseudoRandomKeyGenerator<uint64_t> key_generator_064{1234};
  boardstate::PseudoRandomKeyGenerator<__uint128_t> key_generator_128{1234};
}

//! Include this test to demonstrate peculiarities of std::mt19937 output size.
//! Output is really 32 bits, but uses std::uint_fast32_t which is 64-bit int on 64-bit
//! systems. When value is stored in a 64 bit int, still only uses lower 32 bits.
TEST_F(PseudoRandomKeyGeneratorTest, CheckMt19937OutputSize) {
  std::mt19937 gen_{1234};
  auto my_num = gen_();
  boardstate::PrintHex(my_num);
  auto output_size = sizeof(my_num);
  std::cout << "Generator output size = " << output_size << std::endl;
}

TEST_F(PseudoRandomKeyGeneratorTest, GenerateAndPrintKeysNoSeed) {

  boardstate::PseudoRandomKeyGenerator<uint32_t> key_generator_032{};
  auto random_032 = key_generator_032.GenerateKey();
  auto random_032_string = boardstate::IntToHexString(random_032);
  boardstate::PrintHex(random_032);
  std::cout << random_032_string << std::endl;

  boardstate::PseudoRandomKeyGenerator<uint64_t> key_generator_064{};
  auto random_064 = key_generator_064.GenerateKey();
  auto random_064_string = boardstate::IntToHexString(random_064);
  boardstate::PrintHex(random_064);
  std::cout << random_064_string << std::endl;

  boardstate::PseudoRandomKeyGenerator<__uint128_t> key_generator_128{};
  auto random_128 = key_generator_128.GenerateKey();
  auto random_128_string = boardstate::IntToHexString(random_128);
  boardstate::PrintHex(random_128);
  std::cout << random_128_string << std::endl;

}

TEST_F(PseudoRandomKeyGeneratorTest, GenerateAndPrintKeysWithSeed) {

  boardstate::PseudoRandomKeyGenerator<uint32_t> key_generator_032{1234};
  auto random_032 = key_generator_032.GenerateKey();
  boardstate::PrintHex(random_032);

  boardstate::PseudoRandomKeyGenerator<uint64_t> key_generator_064{1234};
  auto random_064 = key_generator_064.GenerateKey();
  boardstate::PrintHex(random_064);

  boardstate::PseudoRandomKeyGenerator<__uint128_t> key_generator_128{1234};
  auto random_128 = key_generator_128.GenerateKey();
  boardstate::PrintHex(random_128);
}

class RandomKeyGeneratorTest : public ::testing::Test {
protected:
};

TEST_F(RandomKeyGeneratorTest, Instantiate) {
  boardstate::RandomKeyGenerator<uint32_t> key_generator_032{};
  boardstate::RandomKeyGenerator<uint64_t> key_generator_064{};
  boardstate::RandomKeyGenerator<__uint128_t> key_generator_128{};
}

TEST_F(RandomKeyGeneratorTest, GenerateAndPrintKeys) {

  boardstate::RandomKeyGenerator<uint32_t> key_generator_032{};
  auto random_032 = key_generator_032.GenerateKey();
  boardstate::PrintHex(random_032);

  boardstate::RandomKeyGenerator<uint64_t> key_generator_064{};
  auto random_064 = key_generator_064.GenerateKey();
  boardstate::PrintHex(random_064);

  boardstate::RandomKeyGenerator<__uint128_t> key_generator_128{};
  auto random_128 = key_generator_128.GenerateKey();
  boardstate::PrintHex(random_128);
}
