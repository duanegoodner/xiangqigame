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
    auto result = KeyGenerator::generate_key<uint64_t>(gen_64_);
    KeyGenerator::display_key<uint64_t>(result);
}

TEST_F(KeyGeneratorTest, Generate128BitKey) {
    auto result = KeyGenerator::generate_key<__uint128_t>(gen_64_);
    KeyGenerator::display_key<__uint128_t>(result);
}

TEST_F(KeyGeneratorTest, GenerateZKeyTypeKey) {
  zkey_t result = KeyGenerator::generate_zkey(gen_64_);
  KeyGenerator::display_zkey(result);
}
