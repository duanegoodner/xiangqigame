#include <board_data_structs.hpp>
#include <gtest/gtest.h>
#include <move_translator.hpp>

class MoveTranslatorTest : public ::testing::Test {
protected:
  std::string valid_input_a_{"b1, c3"};
  std::string invalid_input_b_{"b1c3"};

  std::istringstream valid_input_stream_a_;
  std::istringstream invalid_input_stream_b_;

  std::vector<std::string> valid_parsed_input_a_{"b1", "c3"};
  std::vector<std::string> invalid_parsed_input_b_{"b1c3"};

  gameboard::BoardSpace board_space_a_start_{9, 1};
  gameboard::BoardSpace board_space_a_end_{7, 2};
  gameboard::Move move_a_{board_space_a_start_, board_space_a_end_};

  void SetUp() override {
    valid_input_stream_a_.str(valid_input_a_); // Set the input content
    invalid_input_stream_b_.str(invalid_input_b_);
  }
};

TEST_F(MoveTranslatorTest, TestGetInput) {
  auto valid_input = movetranslation::GetInput(valid_input_stream_a_);
  EXPECT_EQ(valid_input, valid_input_a_);

  auto invalid_input = movetranslation::GetInput(invalid_input_stream_b_);
  EXPECT_EQ(invalid_input, invalid_input_b_);
}

TEST_F(MoveTranslatorTest, TestParseInput) {
  auto valid_parsed_input = movetranslation::ParseInput(valid_input_a_);
  EXPECT_EQ(valid_parsed_input.size(), 2);
  EXPECT_EQ(valid_parsed_input[0], "b1");
  EXPECT_EQ(valid_parsed_input[1], "c3");

  auto invalid_parsed_input = movetranslation::ParseInput(invalid_input_b_);
  EXPECT_EQ(invalid_parsed_input.size(), 1);
  EXPECT_EQ(invalid_parsed_input[0], "b1c3");
}

TEST_F(MoveTranslatorTest, TestAlbebraicPairValidation) {
  EXPECT_TRUE(movetranslation::IsValidAlgebraicPair(valid_parsed_input_a_));
  EXPECT_FALSE(movetranslation::IsValidAlgebraicPair(invalid_parsed_input_b_));
}

TEST_F(MoveTranslatorTest, TestAlbgebraicSpaceToBoardSpace) {
  auto algebraic_space_a_start = valid_parsed_input_a_[0];
  auto board_space_a_start =
      movetranslation::AlgebraicSpaceToBoardSpace(algebraic_space_a_start);
  EXPECT_EQ(board_space_a_start, board_space_a_start_);

  auto algebraic_space_a_end = valid_parsed_input_a_[1];
  auto board_space_a_end =
      movetranslation::AlgebraicSpaceToBoardSpace(algebraic_space_a_end);
  EXPECT_EQ(board_space_a_end, board_space_a_end_);
}

TEST_F(MoveTranslatorTest, TestBoardSpaceToAlgebraicSpace) {
  auto computed_algebraic_space_a_start =
      movetranslation::BoardSpaceToAlgebraicSpace(board_space_a_start_);
  EXPECT_EQ(computed_algebraic_space_a_start, "b1");

  auto computed_algebraic_space_a_end =
      movetranslation::BoardSpaceToAlgebraicSpace(board_space_a_end_);
  EXPECT_EQ(computed_algebraic_space_a_end, "c3");
}

TEST_F(MoveTranslatorTest, TestConvertParsedInputToMove) {
  auto calculated_move =
      movetranslation::ConvertParsedInputToMove(valid_parsed_input_a_);
  EXPECT_EQ(calculated_move, move_a_);
}

TEST_F(MoveTranslatorTest, TestConvertMoveToInputString) {
  auto computed_input_string = movetranslation::ConvertMoveToInputString(move_a_);
  EXPECT_EQ(computed_input_string, valid_input_a_);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
