#include <board_data_structs.hpp>
#include <game_board_for_concepts.hpp>
#include <memory>
#include <piece_moves.hpp>
#include <stdexcept>
#include <utility_functs.hpp>

namespace gameboard {

//! Starting board represented as 2-D array of integers.
//! Can be converted to array of GamePiece objects by
//! board_utilities::int_board_to_game_pieces.
const BoardMapInt_t kStandardInitialBoard = {{
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

//! Max allowed repetitions of prohibited move sequence lengths.
const int kRepeatPeriodsToCheck[3] = {2, 3, 4};

//! Repeated move sequence lengths forbidden under move repetition rules.
//! If kRepeatPeriodsToCheck = {2, 3, 4} and kRepeatPeriodsMaxAllowed = 2, then the
//! following sequences are probibited:
//! ABABAB, ABCABCABC, ABCDABCDABCD, ABCDEABCDEABCDE, ABCDEFABCDEFABCDEF
const int kRepeatPeriodsMaxAllowed = 2;

const int kMaxMovesWithoutCapture = 120;

//! Initializes a gameboard::GameBoard from array of pieces represented as integers.
//! @param starting_board An array of integers representing pieces on the board.
GameBoardForConcepts::GameBoardForConcepts(const BoardMapInt_t starting_board)
    : board_map_{int_board_to_game_pieces(starting_board)}
    , move_calculator_{MoveCalculator()}
    , moves_since_last_capture_{} {}

// GameBoardForConcepts::GameBoardForConcepts()
//     : GameBoardForConcepts(kStandardInitialBoard) {}

std::vector<BoardSpace> GameBoardForConcepts::GetAllSpacesOccupiedBy(PieceColor color
) const {
  return get_all_spaces_occupied_by(board_map_, color);
}

PieceColor GameBoardForConcepts::GetColor(const BoardSpace &space) const {
  return get_color(board_map_, space);
}

PieceType GameBoardForConcepts::GetType(const BoardSpace &space) const {
  return get_type(board_map_, space);
}

MoveCollection GameBoardForConcepts::CalcFinalMovesOf(PieceColor color) {
  auto un_tested_moves = move_calculator_.CalcAllMovesNoCheckTest(color, board_map_);
  MoveCollection validated_moves{};

  if (IsDraw()) {
    return validated_moves;
  }

  validated_moves.moves.reserve(un_tested_moves.moves.size());

  for (auto move : un_tested_moves.moves) {
    auto executed_move = ExecuteMove(move);
    auto resulting_opponent_moves =
        move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
    auto resulting_gen_position = get_general_position(board_map_, color);

    if (not resulting_opponent_moves.ContainsDestination(resulting_gen_position) and
        not ViolatesRepeatRule(color)) {
      validated_moves.Append(move);
    }

    UndoMove(executed_move);
  }
  return validated_moves;
}

bool GameBoardForConcepts::IsInCheck(PieceColor color) {
  auto gen_position = get_general_position(board_map_, color);
  auto opponent_moves =
      move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
  return opponent_moves.ContainsDestination(gen_position);
}

ExecutedMove GameBoardForConcepts::ExecuteMove(const Move &move) {
  auto moving_piece = GetOccupantAt(move.start);
  auto destination_piece = GetOccupantAt(move.end);
  SetOccupantAt(move.end, moving_piece);
  SetOccupantAt(move.start, GamePiece(PieceType::kNnn, PieceColor::kNul));

  auto executed_move =
      ExecutedMove{move, moving_piece, destination_piece, moves_since_last_capture_};
  UpdateStateTracker(executed_move);
  AddToMoveLog(executed_move);
  if (!IsCaptureMove(executed_move)) {
    moves_since_last_capture_++;
  } else {
    moves_since_last_capture_ = 0;
  }

  return executed_move;
};

bool GameBoardForConcepts::IsCaptureMove(const ExecutedMove &executed_move) const {
  return executed_move.destination_piece != PieceColor::kNul;
}

void GameBoardForConcepts::UndoMove(const ExecutedMove &executed_move) {
  SetOccupantAt(executed_move.spaces.start, executed_move.moving_piece);
  SetOccupantAt(executed_move.spaces.end, executed_move.destination_piece);
  UpdateStateTracker(executed_move);
  moves_since_last_capture_ = executed_move.moves_since_last_capture;
  RemoveFromMoveLog(executed_move);
};

GamePiece GameBoardForConcepts::GetOccupantAt(const BoardSpace &space) const {
  return board_map_[space.rank][space.file];
};

const BoardMap_t &GameBoardForConcepts::map() const { return board_map_; }

void GameBoardForConcepts::AttachMoveCallback(
    const std::function<void(const ExecutedMove &)> &callback
) {
  move_callbacks_.emplace_back(callback);
}

bool GameBoardForConcepts::IsDraw() {
  return moves_since_last_capture_ >= kMaxMovesWithoutCapture;
}

const std::map<PieceColor, std::vector<ExecutedMove>> &GameBoardForConcepts::move_log(
) const {
  return move_log_;
}

void GameBoardForConcepts::UpdateStateTracker(const ExecutedMove &executed_move) {
  for (const auto &callback : move_callbacks_) {
    callback(executed_move);
  }
};

void GameBoardForConcepts::SetOccupantAt(const BoardSpace &space, GamePiece piece) {
  board_map_[space.rank][space.file] = piece;
}

void GameBoardForConcepts::AddToMoveLog(const ExecutedMove &executed_move) {
  auto piece_color = executed_move.moving_piece.piece_color;
  move_log_[piece_color].push_back(executed_move);
};

void GameBoardForConcepts::RemoveFromMoveLog(const ExecutedMove &executed_move) {
  auto piece_color = executed_move.moving_piece.piece_color;
  auto last_move_by_color = move_log_[piece_color].back();
  if (!(executed_move == last_move_by_color)) {
    throw runtime_error("Last move in log does not match move to be removed");
  }
  move_log_[piece_color].pop_back();
}

bool GameBoardForConcepts::ViolatesRepeatRule(PieceColor color) {
  for (auto period_length : kRepeatPeriodsToCheck) {
    auto lookback_length = (kRepeatPeriodsMaxAllowed + 1) * period_length;
    if (utility_functs::hasRepeatingPattern(
            move_log_[color],
            lookback_length,
            period_length
        )) {
      return true;
    }
  }
  return false;
}

std::shared_ptr<GameBoardForConcepts> GameBoardBuilder::build(
    const BoardMapInt_t starting_board
) {
  return std::make_shared<GameBoardForConcepts>(starting_board);
}

} // namespace gameboard
