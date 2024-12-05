#pragma once

#include <board_data_structs.hpp>
#include <move_data_structs.hpp>

namespace fixtures {

class BoardMapFixture {
protected:
  const gameboard::BoardMapInt_t kStartingBoard = {{
      {5, 4, 3, 2, 1, 2, 3, 4, 5},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 6, 0, 0, 0, 0, 0, 6, 0},
      {7, 0, 7, 0, 7, 0, 7, 0, 7},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {-7, 0, -7, 0, -7, 0, -7, 0, -7},
      {0, -6, 0, 0, 0, 0, 0, -6, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {-5, -4, -3, -2, -1, -2, -3, -4, -5},
  }};

  gameboard::BoardMap_t starting_boardmap_ = int_board_to_game_pieces(kStartingBoard);

public:
  gameboard::BoardMap_t starting_boardmap() { return starting_boardmap_; }
  gameboard::ExecutedMove GenerateExecutedMove(
      const BoardSpace &move_start,
      const BoardSpace &move_end
  ) {
    auto moving_piece = starting_boardmap_[move_start.rank][move_start.file];
    auto destination_piece = starting_boardmap_[move_end.rank][move_end.file];
    gameboard::Move move{move_start, move_end};

    return gameboard::ExecutedMove{move, moving_piece, destination_piece};
  }
};

} // namespace fixtures