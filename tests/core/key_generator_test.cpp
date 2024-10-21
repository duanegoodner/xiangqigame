// #include <common.hpp>
#include <gtest/gtest.h>
#include <key_generator.hpp>
#include <random>

class KeyGeneratorTest : public ::testing::Test {
protected:
};

TEST_F(KeyGeneratorTest, InstantiateClass) {
  boardstate::KeyGenerator key_generator;
}

TEST_F(KeyGeneratorTest, ClassGenerate64BitKey) {
  boardstate::KeyGenerator key_generator;
  auto key_64 = key_generator.GenerateKey<uint64_t>();
  key_generator.DisplayKey(key_64);
}

TEST_F(KeyGeneratorTest, ClassGenerate128BitKey) {
  boardstate::KeyGenerator key_generator;
  auto key_128 = key_generator.GenerateKey<__uint128_t>();
  key_generator.DisplayKey(key_128);
}


class KeyGeneratorNewTest : public ::testing::Test {
  protected:
};

TEST_F(KeyGeneratorNewTest, Instantiate) {
  boardstate::KeyGeneratorNew<uint32_t> key_generator_032{};
  boardstate::KeyGeneratorNew<uint64_t> key_generator_064{};
  boardstate::KeyGeneratorNew<__uint128_t> key_generator_128{}; 
}

TEST_F(KeyGeneratorNewTest, GenerateAndPrintKeys) {
  
  boardstate::KeyGeneratorNew<uint32_t> key_generator_032{};
  auto random_032 = key_generator_032.GenerateKey();
  boardstate::PrintHex(random_032);
  
  boardstate::KeyGeneratorNew<uint64_t> key_generator_064{};
  auto random_064 = key_generator_064.GenerateKey();
  boardstate::PrintHex(random_064);

  boardstate::KeyGeneratorNew<__uint128_t> key_generator_128{};
  auto random_128 = key_generator_128.GenerateKey();
  boardstate::PrintHex(random_128);
}


