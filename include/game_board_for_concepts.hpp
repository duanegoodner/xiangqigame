#pragma once

#include <board_data_structs.hpp>
#include <concept_board_state_calculator.hpp>
// #include <concept_board_state_tracker.hpp>
#include <functional>
#include <map>
#include <memory>
#include <move_calculator.hpp>
#include <move_data_structs.hpp>

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

//! Must comply with SpaceInfoProviderConcept; stores piece positions, and exposes
//! methods for calculating, executing, an un-doing moves..

template <BoardStateCalculatorConcept RC, BoardStateCalculatorConcept BC>
class GameBoardForConcepts {
  //! 2-D array of GamePiece objects.
  BoardMap_t board_map_;

  //! Encapsulates all calculations of allowed moves.
  MoveCalculator move_calculator_;

  std::vector<std::shared_ptr<RC>> red_calculators_;
  std::vector<std::shared_ptr<BC>> black_calculators_;
  //! Vectors of all moves that have been executed (and not un-done) by each player.
  std::map<PieceColor, std::vector<ExecutedMove>> move_log_;

  //! Number of moves executed since last time a piece was captured.
  MoveCountType moves_since_last_capture_;

public:
  const BoardMap_t &map() const { return board_map_; }
  const std::map<PieceColor, std::vector<ExecutedMove>> &move_log() const {
    return move_log_;
  }

  static std::shared_ptr<GameBoardForConcepts<RC, BC>> Create(
      std::vector<std::shared_ptr<RC>> red_calclulators,
      std::vector<std::shared_ptr<BC>> black_calculators,
      const BoardMapInt_t starting_board = kStandardInitialBoard
  ) {
    return std::shared_ptr<GameBoardForConcepts<RC, BC>>(
        new GameBoardForConcepts<RC, BC>(
            red_calclulators,
            black_calculators,
            starting_board
        )
    );
  }

  static ::shared_ptr<GameBoardForConcepts<RC, BC>> CreateWithoutZCalculators(
      const BoardMapInt_t starting_board = kStandardInitialBoard
  ) {
    std::vector<std::shared_ptr<RC>> red_calculators;
    std::vector<std::shared_ptr<BC>> black_calculators;
    return std::shared_ptr<GameBoardForConcepts<RC, BC>>(
        new GameBoardForConcepts<RC, BC>(
            red_calculators,
            black_calculators,
            starting_board
        )
    );
  }

  template <BoardStateCalculatorConcept C>
  void AttachCalculator(std::shared_ptr<C> calculator, PieceColor color) {
    if (color == PieceColor::kRed) {
      red_calculators_.emplace_back(calculator);
    }
    if (color == PieceColor::kBlk) {
      black_calculators_.emplace_back(calculator);
    }
  }

  PieceColor GetColor(const BoardSpace &space) const { return GetColorInternal(space); }
  bool IsInCheck(PieceColor color) { return IsInCheckInternal(color); }
  ExecutedMove ExecuteMove(const Move &move) { return ExecuteMoveInternal(move); }
  bool IsDraw() { return IsDrawInternal(); }

  std::vector<BoardSpace> GetAllSpacesOccupiedBy(const PieceColor color) const {
    return GetAllSpacesOccupiedByInternal(color);
  }

  PieceType GetType(const BoardSpace &space) const { return GetTypeInternal(space); }
  MoveCollection CalcFinalMovesOf(PieceColor color) {
    return CalcFinalMovesOfInternal(color);
  }

  bool IsCaptureMove(const ExecutedMove &executed_move) const {
    return IsCaptureMoveInternal(executed_move);
  }

  void UndoMove(const ExecutedMove &executed_move) {
    return UndoMoveInternal(executed_move);
  }
  GamePiece GetOccupantAt(const BoardSpace &space) const {
    return GetOccupantAtInternal(space);
  }

private:
  //! Initializes a gameboard::GameBoard from array of pieces represented as integers.
  //! @param starting_board An array of integers representing pieces on the board.
  GameBoardForConcepts(
      std::vector<std::shared_ptr<RC>> red_calclulators,
      std::vector<std::shared_ptr<BC>> black_calculators,
      const BoardMapInt_t starting_board
  )
      : board_map_{int_board_to_game_pieces(starting_board)}
      , red_calculators_{red_calclulators}
      , black_calculators_{black_calculators}
      , move_calculator_{MoveCalculator()}
      , moves_since_last_capture_{} {}
  inline PieceColor GetColorInternal(const BoardSpace &space) const {
    return board_map_[space.rank][space.file].piece_color;
  }

  inline PieceType GetTypeInternal(const BoardSpace &space) const {
    return board_map_[space.rank][space.file].piece_type;
  }

  inline vector<BoardSpace> GetAllSpacesOccupiedByInternal(const PieceColor color
  ) const {
    vector<BoardSpace> occupied_spaces;
    occupied_spaces.reserve(16);
    for (auto rank = 0; rank < kNumRanks; rank++) {
      for (auto file = 0; file < kNumFiles; file++) {
        if (GetColorInternal(BoardSpace{rank, file}) == color) {
          occupied_spaces.emplace_back(BoardSpace{rank, file});
        }
      }
    }
    return occupied_spaces;
  }

  inline GamePiece GetOccupantAtInternal(const BoardSpace &space) const {
    return board_map_[space.rank][space.file];
  }

  inline BoardSpace GetGeneralPositionInternal(const PieceColor color) {
    auto castle =
        (color == PieceColor::kRed) ? red_castle_spaces() : black_castle_spaces();

    BoardSpace found_space;

    for (BoardSpace board_space : castle) {
      auto piece = board_map_[board_space.rank][board_space.file];
      if (piece.piece_type == PieceType::kGen) {
        found_space = board_space;
      }
    }
    return found_space;
  }

  MoveCollection CalcFinalMovesOfInternal(PieceColor color) {
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

  bool IsInCheckInternal(PieceColor color) {
    auto gen_position = get_general_position(board_map_, color);
    auto opponent_moves =
        move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
    return opponent_moves.ContainsDestination(gen_position);
  }

  ExecutedMove ExecuteMoveInternal(const Move &move) {
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
  }

  bool IsCaptureMoveInternal(const ExecutedMove &executed_move) const {
    return executed_move.destination_piece != PieceColor::kNul;
  }

  void UndoMoveInternal(const ExecutedMove &executed_move) {
    SetOccupantAt(executed_move.spaces.start, executed_move.moving_piece);
    SetOccupantAt(executed_move.spaces.end, executed_move.destination_piece);
    UpdateStateTracker(executed_move);
    moves_since_last_capture_ = executed_move.moves_since_last_capture;
    RemoveFromMoveLog(executed_move);
  };

  void UpdateStateTracker(const ExecutedMove &executed_move) {

    for (auto z_calculator : red_calculators_) {
      z_calculator->UpdateBoardState(executed_move);
    }
    for (auto z_calculator : black_calculators_) {
      z_calculator->UpdateBoardState(executed_move);
    }
  }

  void SetOccupantAt(const BoardSpace &space, GamePiece piece) {
    board_map_[space.rank][space.file] = piece;
  }

  void AddToMoveLog(const ExecutedMove &executed_move) {
    auto piece_color = executed_move.moving_piece.piece_color;
    move_log_[piece_color].push_back(executed_move);
  }

  void RemoveFromMoveLog(const ExecutedMove &executed_move) {
    auto piece_color = executed_move.moving_piece.piece_color;
    auto last_move_by_color = move_log_[piece_color].back();
    if (!(executed_move == last_move_by_color)) {
      throw runtime_error("Last move in log does not match move to be removed");
    }
    move_log_[piece_color].pop_back();
  }

  bool ViolatesRepeatRule(PieceColor color) {
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

  bool IsDrawInternal() { return moves_since_last_capture_ >= kMaxMovesWithoutCapture; }
};

} // namespace gameboard