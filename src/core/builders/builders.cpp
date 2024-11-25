#include <builders.hpp>

namespace gameboard {


// std::shared_ptr<GameBoardForConcepts> GameBoardBuilder::build(
//     const BoardMapInt_t starting_board
// ) {
//   return std::make_shared<GameBoardForConcepts>(starting_board);
// }

} // namespace gameboard


namespace piecepoints {
    std::shared_ptr<PiecePositionPointsForConcepts> PiecePositionPointsBuilder::build(
    std::string json_file
) {
  piecepoints::BPOPointsSKeys bpo_points_skeys{json_file};
  auto game_points_array = bpo_points_skeys.ToGamePointsArray();
  return std::make_shared<PiecePositionPointsForConcepts>(game_points_array);
}
}