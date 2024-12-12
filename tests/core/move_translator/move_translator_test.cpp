#include <board_data_structs.hpp>
#include <gtest/gtest.h>
#include <move_translator.hpp>
#include <ranges>
#include <sstream>

class MoveTranslatorTest : public ::testing::Test {
protected:
  std::string good_input_string_{"b1, c3"};
  std::string bad_input_string_{"b1c3"};

  std::istringstream good_input_stream_;
  std::istringstream bad_input_stream_;

  std::vector<std::string> good_tokens_{"b1", "c3"};
  std::vector<std::string> bad_tokens_{"b1c3"};

  gameboard::BoardSpace board_space_a_start_{9, 1};
  gameboard::BoardSpace board_space_a_end_{7, 2};
  gameboard::Move move_a_{board_space_a_start_, board_space_a_end_};

  void SetUp() override {
    good_input_stream_.str(good_input_string_); // Set the input content
    bad_input_stream_.str(bad_input_string_);
  }
};

TEST_F(MoveTranslatorTest, TestGetInput) {
  auto valid_input = movetranslation::GetInput(good_input_stream_);
  EXPECT_EQ(valid_input, good_input_string_);

  auto invalid_input = movetranslation::GetInput(bad_input_stream_);
  EXPECT_EQ(invalid_input, bad_input_string_);
}

TEST_F(MoveTranslatorTest, TestTokenize) {
  auto tokenized_good_input = movetranslation::Tokenize(good_input_string_);
  EXPECT_EQ(tokenized_good_input.size(), good_tokens_.size());

  
  for (auto idx = 0; idx < tokenized_good_input.size(); ++idx) {
    EXPECT_EQ(tokenized_good_input.at(idx), good_tokens_.at(idx));
  }

  auto tokenized_bad_input = movetranslation::Tokenize(bad_input_string_);
  EXPECT_EQ(tokenized_bad_input.size(), 1);
  EXPECT_EQ(tokenized_bad_input.at(0), bad_tokens_.at(0));
}

TEST_F(MoveTranslatorTest, TestIsValidAlgebraicBoardSpace) {
  
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
