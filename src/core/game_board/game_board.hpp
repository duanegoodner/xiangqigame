// Filename: game_board.hpp
// Author: Duane Goodner
// Created: 2022-11-14
// Last Modified: 2024-08-16

// Description:
// Definition of GameBoard class, and the BoardStateSummarizer CRTP interface
// that a GameBoard requires a HashCalculator to comply with.

#ifndef _GAME_BOARD_
#define _GAME_BOARD_

#include <board_components.hpp>
#include <common.hpp>
#include <game_board_details.hpp>
#include <move_calculator.hpp>
#include <move_evaluators.hpp>
#include <vector>

using namespace std;
using namespace board_components;

// CRTP INTERFACE: GameBoard <- BoardStateSummarizer (concrete example =
// HashCalculator)
template <typename ConcreteBoardStateSummarizer, typename KeyType>
class BoardStateSummarizer {
public:
  typedef KeyType ZobristKey_t;
  void CalcInitialBoardState(BoardMap_t &board_map) {
    static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementCalcInitialBoardState(
        board_map
    );
  }

  void CalcNewBoardState(const ExecutedMove &move) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementCalcNewBoardState(
        move
    );
  }

  ZobristKey_t GetState() {
    return static_cast<ConcreteBoardStateSummarizer *>(this)->ImplementGetState();
  }

  void RecordCurrentStateMinimaxResult(
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)
        ->ImplementRecordCurrentStateMinimaxResult(
            search_depth,
            result_type,
            best_moves
        );
  }

  TranspositionTableSearchResult GetCurrentStateMinimaxResult(int search_depth) {
    return static_cast<ConcreteBoardStateSummarizer *>(this)
        ->ImplementGetCurrentStateMinimaxResult(search_depth);
  }
};

// Template for class NewGameBoard which implements interface
// SpaceInfoProvider, and uses a ConcreteBoardStateSummarizer
template <
    typename ConcreteBoardStateSummarizerRed,
    typename ConcreteBoardStateSummarizerBlack>
class NewGameBoard : public SpaceInfoProvider<NewGameBoard<
                         ConcreteBoardStateSummarizerRed,
                         ConcreteBoardStateSummarizerBlack>> {
public:
  NewGameBoard();
  NewGameBoard(const BoardMapInt_t starting_board);
  vector<BoardSpace> ImplementGetAllSpacesOccupiedBy(PieceColor color) {
    return get_all_spaces_occupied_by(board_map_, color);
  }
  PieceColor ImplementGetColor(BoardSpace space) { return get_color(board_map_, space); }
  PieceType ImplementGetType(BoardSpace space) { return get_type(board_map_, space); }
  TranspositionTableSearchResult ImplementSearchTranspositionTable(
      PieceColor color,
      int search_depth
  ) {
    return _ImplementSearchTranspositionTable(color, search_depth);
  }

  void ImplementRecordCurrentStateScore(
      PieceColor color,
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    _ImplementRecordCurrentStateScore(color, search_depth, result_type, best_moves);
  };

  MoveCollection ImplementCalcFinalMovesOf(PieceColor color) {
    return _ImplementCalcFinalMovesOf(color);
  }
  bool IsInCheck(PieceColor color) {
    auto gen_position = get_general_position(board_map_, color);
    auto opponent_moves =
        move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
    return opponent_moves.ContainsDestination(gen_position);
  }
  ExecutedMove ImplementExecuteMove(Move move) {
    auto moving_piece = GetOccupant(move.start);
    auto destination_piece = GetOccupant(move.end);
    SetOccupant(move.end, moving_piece);
    SetOccupant(move.start, GamePiece(PieceType::kNnn, PieceColor::kNul));

    auto executed_move = ExecutedMove{move, moving_piece, destination_piece};
    UpdateHashCalculator(executed_move);
    AddToMoveLog(executed_move);

    return ExecutedMove{move, moving_piece, destination_piece};
  };
  void ImplementUndoMove(ExecutedMove executed_move) {
    return PrivateImplementUndoMove(executed_move);
  };
  GamePiece GetOccupant(BoardSpace space) { return board_map_[space.rank][space.file]; };
  const BoardMap_t &map() const { return board_map_; }

private:
  BoardMap_t board_map_;
  MoveCalculator move_calculator_;
  ConcreteBoardStateSummarizerRed hash_calculator_red_;
  ConcreteBoardStateSummarizerBlack hash_calculator_black_;

  void PrivateImplementUndoMove(ExecutedMove executed_move) {
    SetOccupant(executed_move.spaces.start, executed_move.moving_piece);
    SetOccupant(executed_move.spaces.end, executed_move.destination_piece);
    UpdateHashCalculator(executed_move);
    RemoveFromMoveLog(executed_move);
  }

  // Retrieve info for current state //////////////////////////////////////////
  TranspositionTableSearchResult GetCurrentStateDetailsRed(int search_depth) {
    return hash_calculator_red_.GetCurrentStateMinimaxResult(search_depth);
  };
  TranspositionTableSearchResult GetCurrentStateDetailsBlack(int search_depth) {
    return hash_calculator_black_.GetCurrentStateMinimaxResult(search_depth);
  }
  unordered_map<PieceColor, TranspositionTableSearchResult (NewGameBoard::*)(int)>
      state_details_dispatch_table_;
  TranspositionTableSearchResult _ImplementSearchTranspositionTable(
      PieceColor color,
      int search_depth
  ) {
    auto search_function = state_details_dispatch_table_.at(color);
    return (this->*search_function)(search_depth);
  }
  //////////////////////////////////////////////////////////////////////////////

  // Record info for current state/////////////////////////////////////////////
  void RecordCurrentStateDetailsRed(
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    hash_calculator_red_
        .RecordCurrentStateMinimaxResult(search_depth, result_type, best_moves);
  };
  void RecordCurrentStateDetailsBlack(
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    hash_calculator_black_
        .RecordCurrentStateMinimaxResult(search_depth, result_type, best_moves);
  };

  unordered_map<PieceColor, void (NewGameBoard::*)(int, MinimaxResultType, BestMoves &)>
      write_state_details_dispatch_table_;

  void _ImplementRecordCurrentStateScore(
      PieceColor color,
      int search_depth,
      MinimaxResultType result_type,
      BestMoves &best_moves
  ) {
    auto record_function = write_state_details_dispatch_table_.at(color);
    (this->*record_function)(search_depth, result_type, best_moves);
  }
  /////////////////////////////////////////////////////////////////////////////////

  std::map<PieceColor, vector<ExecutedMove>> move_log_;
  void UpdateHashCalculator(ExecutedMove executed_move) {
    hash_calculator_red_.CalcNewBoardState(executed_move);
    hash_calculator_black_.CalcNewBoardState(executed_move);
  };
  void SetOccupant(BoardSpace space, GamePiece piece) {
    board_map_[space.rank][space.file] = piece;
  }
  void AddToMoveLog(ExecutedMove executed_move) {
    auto piece_color = executed_move.moving_piece.piece_color;
    move_log_[piece_color].push_back(executed_move);
  };
  void RemoveFromMoveLog(ExecutedMove executed_move) {
    auto piece_color = executed_move.moving_piece.piece_color;
    auto last_move_by_color = move_log_[piece_color].back();
    if (!(executed_move == last_move_by_color)) {
      throw runtime_error("Last move in log does not match move to be removed");
    }
    move_log_[piece_color].pop_back();
  }
  bool ViolatesRepeatRule(PieceColor color) {
    for (auto period_length : kRepeatPeriodsToCheck) {
      auto lookback_length = (kMaxAllowedRepeatPeriods + 1) * period_length;
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
  MoveCollection _ImplementCalcFinalMovesOf(PieceColor color) {
    auto un_tested_moves = move_calculator_.CalcAllMovesNoCheckTest(color, board_map_);
    MoveCollection validated_moves;
    validated_moves.moves.reserve(un_tested_moves.moves.size());

    for (auto move : un_tested_moves.moves) {
      auto executed_move = ImplementExecuteMove(move);
      auto resulting_opponent_moves =
          move_calculator_.CalcAllMovesNoCheckTest(opponent_of(color), board_map_);
      auto resulting_gen_position = get_general_position(board_map_, color);

      if (not resulting_opponent_moves.ContainsDestination(resulting_gen_position) and
          not ViolatesRepeatRule(color)) {
        validated_moves.Append(move);
      }

      ImplementUndoMove(executed_move);
    }
    return validated_moves;
  }
};

#include <game_board.tpp>

#endif // _GAME_BOARD_