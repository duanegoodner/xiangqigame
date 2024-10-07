#include <common.hpp>
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


