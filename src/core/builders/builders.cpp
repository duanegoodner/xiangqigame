#include <builders.hpp>

namespace gameboard {

std::shared_ptr<GameBoardForConcepts> GameBoardBuilder::build(
    const BoardMapInt_t starting_board
) {
  return std::make_shared<GameBoardForConcepts>(starting_board);
}

} // namespace gameboard