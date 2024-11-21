#include <game_board_for_concepts.hpp>
#include <gtest/gtest.h>
#include <space_info_provider_new_concept.hpp>
#include <type_traits>

class GameBoardForConceptsTest : public ::testing::Test {
protected:
  gameboard::GameBoardForConcepts gb_;
  const gameboard::BoardMapInt_t kRepeatMoveTestBoard{{
      {0, 0, 0, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 7, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 5},
      {5, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, -1, 0, 0, 0, 0},
  }};

  const gameboard::BoardMapInt_t kDrawTestBoard{{
      {0, 0, 0, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {-7, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {7, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, -1, 0, 0, 0, 0},
  }};
};

TEST_F(GameBoardForConceptsTest, SatisfiesSpaceInfoProviderConcept) {
  static_assert(
      SpaceInfoProviderConcept<gameboard::GameBoardForConcepts>,
      "GameBoardForConcepts must satisfy SpaceInfoProviderConcept"
  );
}

TEST_F(GameBoardForConceptsTest, DetectsDraw) {
  GameBoardForConcepts draw_game_board{kDrawTestBoard};

  for (auto round_trip = 0; round_trip < 10; ++round_trip) {
    for (auto idx = 0; idx < 6; ++idx) {
      BoardSpace start{3, idx};
      BoardSpace end{3, idx + 1};
      Move move{start, end};
      draw_game_board.ExecuteMove(move);
    }

    for (auto idx = 6; idx > 0; idx--) {
      BoardSpace start{3, idx};
      BoardSpace end{3, idx - 1};
      Move move{start, end};
      draw_game_board.ExecuteMove(move);
    }
  }
  auto is_draw = draw_game_board.IsDraw();
  auto red_available_moves = draw_game_board.CalcFinalMovesOf(PieceColor::kRed);
  std::cout << draw_game_board.IsDraw() << std::endl;
}

TEST_F(GameBoardForConceptsTest, GetsCorrectOccupants) {
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{0, 0}), 5);
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{1, 0}), 0);
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{9, 7}), -4);
}

TEST_F(GameBoardForConceptsTest, ExecuteAndUndoActualMove) {
  auto actual_move = Move{BoardSpace{6, 2}, BoardSpace{5, 2}};
  auto actual_executed_move = gb_.ExecuteMove(actual_move);
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{6, 2}), 0);
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{5, 2}), -7);
  gb_.UndoMove(actual_executed_move);
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{6, 2}), -7);
  EXPECT_EQ(gb_.GetOccupantAt(BoardSpace{5, 2}), 0);
}

TEST_F(GameBoardForConceptsTest, CorrecNumSpacesOccupiedByBlack) {
  auto black_spaces = gb_.GetAllSpacesOccupiedBy(PieceColor::kBlk);
  EXPECT_EQ(black_spaces.size(), 16);
}

TEST_F(GameBoardForConceptsTest, CorrecNumSpacesOccupiedByRed) {
  auto red_spaces = gb_.GetAllSpacesOccupiedBy(PieceColor::kRed);
  EXPECT_EQ(red_spaces.size(), 16);
}

TEST_F(GameBoardForConceptsTest, CorrectNumberAvailableMoves) {
  auto black_moves = gb_.CalcFinalMovesOf(PieceColor::kBlk);
  auto red_moves = gb_.CalcFinalMovesOf(PieceColor::kRed);
}

TEST_F(GameBoardForConceptsTest, ProhibitsTripleRepeatMovePeriod_02) {
  GameBoardForConcepts late_game_board(kRepeatMoveTestBoard);
  auto red_king_position_a = BoardSpace{9, 4};
  auto red_king_position_b = BoardSpace{9, 3};

  Move move_x = Move{red_king_position_a, red_king_position_b};
  Move move_y = Move{red_king_position_b, red_king_position_a};

  for (int round_trips = 0; round_trips < 2; round_trips++) {
    late_game_board.ExecuteMove(move_x);
    auto avail_moves_a = late_game_board.CalcFinalMovesOf(PieceColor::kRed);
    EXPECT_TRUE(avail_moves_a.Size() > 0);

    late_game_board.ExecuteMove(move_y);
    auto avail_moves_b = late_game_board.CalcFinalMovesOf(PieceColor::kRed);
    EXPECT_TRUE(avail_moves_b.Size() > 0);
  }
  late_game_board.ExecuteMove(move_x);
  auto avail_moves_c = late_game_board.CalcFinalMovesOf(PieceColor::kRed);
  EXPECT_TRUE(avail_moves_c.Size() == 0);
}

TEST_F(GameBoardForConceptsTest, ProhibitsTripleRepeatMovePeriod_03) {
  GameBoardForConcepts late_game_board(kRepeatMoveTestBoard);
  auto red_king_position_a = BoardSpace{9, 4};
  auto red_king_position_b = BoardSpace{9, 3};
  auto red_king_position_c = BoardSpace{9, 5};

  Move move_w = Move{red_king_position_a, red_king_position_b};
  Move move_x = Move{red_king_position_b, red_king_position_a};
  Move move_y = Move{red_king_position_a, red_king_position_c};
  Move move_z = Move{red_king_position_c, red_king_position_a};

  for (int cycles = 0; cycles < 3; cycles++) {
    late_game_board.ExecuteMove(move_w);
    EXPECT_TRUE(late_game_board.CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
    late_game_board.ExecuteMove(move_x);
    EXPECT_TRUE(late_game_board.CalcFinalMovesOf(PieceColor::kRed).Size() > 0);

    if (cycles < 2) {
      late_game_board.ExecuteMove(move_y);
      EXPECT_TRUE(late_game_board.CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
      late_game_board.ExecuteMove(move_z);
      EXPECT_TRUE(late_game_board.CalcFinalMovesOf(PieceColor::kRed).Size() > 0);
    }
  }

  late_game_board.ExecuteMove(move_y);
  EXPECT_TRUE(late_game_board.CalcFinalMovesOf(PieceColor::kRed).Size() == 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
