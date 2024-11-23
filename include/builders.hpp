#pragma once

#include <game_board_for_concepts.hpp>
#include <memory>

namespace gameboard {

class GameBoardBuilder {
public:
  std::shared_ptr<GameBoardForConcepts> build(
      const BoardMapInt_t starting_board = kStandardInitialBoard
  );
};

}

