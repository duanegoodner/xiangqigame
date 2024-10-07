#include <common.hpp>
#include <gtest/gtest.h>
#include <key_generator.hpp>
#include <random>

class KeyGeneratorTest : public ::testing::Test {
protected:
  std::random_device rd_;
  std::mt19937_64 gen_64_{rd_()};
};

TEST_F(KeyGeneratorTest, Generate64BitKey) {
    auto result = keygenerator::generate_key<uint64_t>(gen_64_);
    keygenerator::display_key<uint64_t>(result);
}

TEST_F(KeyGeneratorTest, Generate128BitKey) {
    auto result = keygenerator::generate_key<__uint128_t>(gen_64_);
    keygenerator::display_key<__uint128_t>(result);
}

TEST_F(KeyGeneratorTest, InstantiateClass) {
  KeyGenerator key_generator;
}

TEST_F(KeyGeneratorTest, ClassGenerate64BitKey) {
  KeyGenerator key_generator;
  auto key_64 = key_generator.GenerateKey<uint64_t>();
  key_generator.DisplayKey(key_64);
}

TEST_F(KeyGeneratorTest, ClassGenerate128BitKey) {
  KeyGenerator key_generator;
  auto key_128 = key_generator.GenerateKey<__uint128_t>();
  key_generator.DisplayKey(key_128);
}


