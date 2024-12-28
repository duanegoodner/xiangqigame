#include <cerrno>
#include <cstring>
#include <fstream>
#include <gameboard/board_data_structs.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <movetranslation/move_translator.hpp>
#include <sstream>
#include <string>
#include <vector>

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

  std::string alg_moves_file_{
      std::string(TEST_DATA_DIR) + "/" + "algebraic_moves.txt"
  };

  std::ifstream alg_moves_file_stream_{alg_moves_file_};

  void SetUp() override {

    good_input_stream_.str(good_input_string_); // Set the input content
    bad_input_stream_.str(bad_input_string_);
  }
};

TEST_F(MoveTranslatorTest, ReadSingleAlgebraicMove) {
  if (!alg_moves_file_stream_.is_open()) {
    std::cerr << "Failed to open the file: " << std::strerror(errno) << std::endl;
  }
  std::string first_line = movetranslation::GetInput(alg_moves_file_stream_);
  std::cout << first_line << std::endl;
}

TEST_F(MoveTranslatorTest, ConvertFileInputLineToAlgebraicMove) {
  if (!alg_moves_file_stream_.is_open()) {
    std::cerr << "Failed to open the file: " << std::strerror(errno) << std::endl;
  }
  std::string first_line = movetranslation::GetInput(alg_moves_file_stream_);
  auto tokens = movetranslation::Tokenize(first_line);
  auto algebraic_move = movetranslation::AlgebraicMove::Create(tokens);
  
  auto expected_move_start = algebraic_move.start();
  auto expected_move_start_str = expected_move_start.value();
  auto expected_move_end = algebraic_move.end();
  auto expected_move_end_str = expected_move_end.value();
  
  EXPECT_EQ(expected_move_start_str, "a4");
  EXPECT_EQ(expected_move_end_str, "a5");
}

TEST_F(MoveTranslatorTest, ReadFullAlgMoveInputFile) {
  if (!alg_moves_file_stream_.is_open()) {
    std::cerr << "Failed to open the file: " << std::strerror(errno) << std::endl;
  }
  
  size_t num_lines_read{0};

  while (alg_moves_file_stream_) {
    std::string cur_line = movetranslation::GetInput(alg_moves_file_stream_);
    if (cur_line.empty()) {
      break;
    }
    auto cur_tokens = movetranslation::Tokenize(cur_line);
    auto cur_algebraic_move = movetranslation::AlgebraicMove::Create(cur_tokens);
    auto cur_gameboard_move = cur_algebraic_move.ToGameBoardMove();
    num_lines_read++;
  }
  EXPECT_EQ(num_lines_read, 25);
}

TEST_F(MoveTranslatorTest, ReadFullFileOfAlgebraicMoves) {}

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
