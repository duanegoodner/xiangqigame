#include <gameboard/board_data_structs.hpp>
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

TEST_F(MoveTranslatorTest, TestTrim) { EXPECT_EQ(movetranslation::Trim(" a1 "), "a1"); }

TEST_F(MoveTranslatorTest, TestTokenize) {
  auto tokenized_good_input = movetranslation::Tokenize(good_input_string_);
  EXPECT_EQ(tokenized_good_input.size(), good_tokens_.size());
  for (auto idx = 0; idx < tokenized_good_input.size(); ++idx) {
    EXPECT_EQ(tokenized_good_input.at(idx), good_tokens_.at(idx));
  }

  auto tokenized_bad_input = movetranslation::Tokenize(bad_input_string_);
  EXPECT_EQ(tokenized_bad_input.size(), bad_tokens_.size());
  for (auto idx = 0; idx < tokenized_bad_input.size(); ++idx) {
    EXPECT_EQ(tokenized_bad_input.at(idx), bad_tokens_.at(idx));
  }
}

TEST_F(MoveTranslatorTest, TestIsValidAlgebraicBoardSpace) {
  for (auto &token : good_tokens_) {
    EXPECT_TRUE(movetranslation::IsValidAlgebraicBoardSpace(token));
  }

  for (auto &token : bad_tokens_) {
    EXPECT_FALSE(movetranslation::IsValidAlgebraicBoardSpace(token));
  }

  EXPECT_FALSE(movetranslation::IsValidAlgebraicBoardSpace("a25"));
}

TEST_F(MoveTranslatorTest, TestIsValidAlgebraicMove) {
  EXPECT_TRUE(movetranslation::IsValidAlgebraicMove(good_tokens_));
  EXPECT_FALSE(movetranslation::IsValidAlgebraicMove(bad_tokens_));

  std::vector<std::string> more_bad_tokens_{"a1", "a2", "a3"};
}

TEST_F(MoveTranslatorTest, TestAlgebraicBoardSpace) {
  auto algebraic_board_space = movetranslation::AlgebraicBoardSpace{"a1"};
  EXPECT_EQ(algebraic_board_space.value(), "a1");

  auto game_board_space = algebraic_board_space.ToGameBoardSpace();
  gameboard::BoardSpace expected_game_board_space{9, 0};
  EXPECT_EQ(game_board_space, expected_game_board_space);
}

TEST_F(MoveTranslatorTest, TestAlgebraicMoveInitFromAlgebraicBoardSpaces) {
  auto algebraic_board_space_a1 = movetranslation::AlgebraicBoardSpace{"a1"};
  auto algebraic_board_space_a2 = movetranslation::AlgebraicBoardSpace{"a2"};
  auto algebraic_move_a1_a2 =
      movetranslation::AlgebraicMove{algebraic_board_space_a1, algebraic_board_space_a2};

  EXPECT_EQ(algebraic_move_a1_a2.start(), algebraic_board_space_a1);
  EXPECT_EQ(algebraic_move_a1_a2.end(), algebraic_board_space_a2);

  auto game_board_move = algebraic_move_a1_a2.ToGameBoardMove();
  gameboard::BoardSpace expected_game_board_move_start{9, 0};
  EXPECT_EQ(game_board_move.start, expected_game_board_move_start);
  gameboard::BoardSpace expected_game_board_move_end{8, 0};
  EXPECT_EQ(game_board_move.end, expected_game_board_move_end);
}

TEST_F(MoveTranslatorTest, TestAlgebraicMoveInitFromCreateMethod) {
  std::vector<std::string> tokens{"b1", "b2"};
  auto algebraic_move_b1_b2 = movetranslation::AlgebraicMove::Create(tokens);

  movetranslation::AlgebraicBoardSpace expected_start{"b1"};
  EXPECT_EQ(algebraic_move_b1_b2.start(), expected_start);

  movetranslation::AlgebraicBoardSpace expected_end{"b2"};
  EXPECT_EQ(algebraic_move_b1_b2.end(), expected_end);

  gameboard::Move game_board_move = algebraic_move_b1_b2.ToGameBoardMove();
  gameboard::BoardSpace expected_game_board_move_start{9, 1};
  EXPECT_EQ(game_board_move.start, expected_game_board_move_start);
  gameboard::BoardSpace expected_game_board_move_end{8, 1};
  EXPECT_EQ(game_board_move.end, expected_game_board_move_end);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
