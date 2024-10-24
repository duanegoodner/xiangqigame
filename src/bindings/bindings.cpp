//! @file bindings.cpp
//! Implements Pybind11 module xiangqi_bindings, exposing C++ classes and methods to
//! Python.

#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <board_data_structs.hpp>
#include <game_board.hpp>
#include <hash_calculator.hpp>
#include <move_evaluators.hpp>
#include <piece_position_points.hpp>
#include <string>

namespace py = pybind11;
using namespace py::literals;
using namespace boardstate;
using namespace gameboard;
using namespace piecepoints;

template <typename KeyType>
void bind_zobrist_keys(py::module_ &m, const std::string &class_name) {
  py::class_<ZobristKeys<KeyType>>(m, class_name.c_str())
      .def(py::init<>())
      .def(py::init<uint32_t>(), "seed"_a)
      .def_readonly("turn_key", &ZobristKeys<KeyType>::turn_key)
      .def_readonly("zarray", &ZobristKeys<KeyType>::zarray);
}

template <typename KeyType>
void bind_hash_calculator(py::module_ &m, const std::string &class_name) {
    py::class_<HashCalculator<KeyType>>(m, class_name.c_str())
    .def("get_zobrist_keys", &HashCalculator<KeyType>::GetZobristKeys);
}

template <typename KeyType>
void bind_minimax_move_evaluator(py::module_ &m, const std::string &class_name) {
  py::class_<
      MinimaxMoveEvaluator<GameBoard, HashCalculator<KeyType>, PiecePositionPoints>>(
      m,
      class_name.c_str()
  )
      .def(
          py::init<PieceColor, int, GameBoard &>(),
          "evaluating_player"_a,
          "starting_search_depth"_a,
          "game_board"_a
      )
      .def(
          "select_move",
          &MinimaxMoveEvaluator<
              GameBoard,
              HashCalculator<KeyType>,
              PiecePositionPoints>::SelectMove
      )
      .def_property_readonly(
          "search_summaries",
          &MinimaxMoveEvaluator<
              GameBoard,
              HashCalculator<KeyType>,
              PiecePositionPoints>::search_summaries
      )
      .def(
          "starting_search_depth",
          &MinimaxMoveEvaluator<
              GameBoard,
              HashCalculator<KeyType>,
              PiecePositionPoints>::StartingSearchDepth
      )
      .def(
          "zobrist_key_size_bits",
          &MinimaxMoveEvaluator<
              GameBoard,
              HashCalculator<KeyType>,
              PiecePositionPoints>::KeySizeBits
      )
      .def(
          "get_hash_calculator",
          &MinimaxMoveEvaluator<
              GameBoard,
              HashCalculator<KeyType>,
              PiecePositionPoints>::GetHashCalculator
      );
}

PYBIND11_MODULE(xiangqi_bindings, m) {

  py::class_<BoardSpace>(m, "BoardSpace")
      .def(py::init<int, int>(), "rank"_a, "file"_a)
      .def_readonly("rank", &BoardSpace::rank)
      .def_readonly("file", &BoardSpace::file);

  py::class_<GamePiece>(m, "GamePiece")
      .def(py::init<>())
      .def(py::init<int>(), "int_piece"_a)
      .def(py::init<PieceType, PieceColor>(), "piece_type"_a, "piece_color"_a)
      .def_readonly("piece_type", &GamePiece::piece_type)
      .def_readonly("piece_color", &GamePiece::piece_color);

  py::class_<Move>(m, "Move")
      .def(py::init<BoardSpace, BoardSpace>(), "start"_a, "end"_a)
      .def_readonly("start", &Move::start)
      .def_readonly("end", &Move::end);

  py::class_<MoveCollection>(m, "MoveCollection")
      .def_readonly("moves", &MoveCollection::moves)
      .def("size", &MoveCollection::Size)
      .def("ContainsMove", &MoveCollection::ContainsMove);

  py::class_<EqualScoreMoves>(m, "EqualScoreMoves")
      .def_readonly("shared_score", &EqualScoreMoves::shared_score)
      .def_readonly("similar_moves", &EqualScoreMoves::similar_moves);

  py::class_<ExecutedMove>(m, "ExecutedMove")
      .def(
          py::init<Move, int, int>(),
          "spaces"_a,
          "moving_piece"_a,
          "destination_piece"_a
      )
      .def_readonly("spaces", &ExecutedMove::spaces)
      .def_readonly("moving_piece", &ExecutedMove::moving_piece)
      .def_readonly("destination_piece", &ExecutedMove::destination_piece);

  py::enum_<PieceColor>(m, "PieceColor")
      .value("kRed", kRed)
      .value("kNul", kNul)
      .value("kBlk", kBlk)
      .export_values();

  py::enum_<PieceType>(m, "PieceType")
      .value("kNnn", kNnn)
      .value("kGen", kGen)
      .value("kAdv", kAdv)
      .value("kEle", kEle)
      .value("kHor", kHor)
      .value("kCha", kCha)
      .value("kCan", kCan)
      .value("kSol", kSol)
      .export_values();

  m.def("size_of_points_type", &size_of_points_type);
  m.def("is_signed_points_type", &is_signed_points_type);

  py::enum_<MinimaxResultType>(m, "MinimaxResultType")
      .value("Unknown", kUnknown)
      .value("TrTableHit", kTrTableHit)
      //   .value("TrTableHitEvaluatorLoses", kTrTableHitEvaluatorLoses)
      //   .value("TrTableHitEvaluatorWins", kTrTableHitEvaluatorWins)
      .value("EvaluatorLoses", kEvaluatorLoses)
      .value("EvaluatorWins", kEvaluatorWins)
      .value("Draw", kDraw)
      .value("FullyEvaluatedNode", kFullyEvaluatedNode)
      .value("StandardLeaf", kStandardLeaf)
      .value("AlphaPrune", kAlphaPrune)
      .value("BetaPrune", kBetaPrune)
      .export_values();

  py::class_<GameBoard>(m, "GameBoard")
      .def(py::init<>())
      .def("map", &GameBoard::map)
      .def("ExecuteMove", &GameBoard::ExecuteMove, "move"_a)
      .def("UndoMove", &GameBoard::UndoMove, "executed_move"_a)
      .def("GetAllSpacesOccupiedBy", &GameBoard::GetAllSpacesOccupiedBy, "color"_a)
      .def("CalcFinalMovesOf", &GameBoard::CalcFinalMovesOf, "color"_a)
      .def("IsInCheck", &GameBoard::IsInCheck, "color"_a)
      .def("GetType", &GameBoard::GetType, "space"_a)
      .def_property_readonly("move_log", &GameBoard::move_log)
      .def_property_readonly("is_draw", &GameBoard::IsDraw)
      .def("GetColor", &GameBoard::GetColor, "space"_a);

  m.def("opponent_of", &opponent_of);

  py::class_<RandomMoveEvaluator<GameBoard>>(m, "RandomMoveEvaluator")
      .def(py::init<PieceColor, GameBoard &>(), "evaluating_player"_a, "game_board"_a)
      .def("select_move", &RandomMoveEvaluator<GameBoard>::SelectMove);

  py::class_<TranspositionTableSize>(m, "TranspositionTableSize")
    .def_readonly("num_entries", &TranspositionTableSize::num_entries)
    .def_readonly("num_states", &TranspositionTableSize::num_states);
  
  py::class_<SearchSummary>(m, "SearchSummary")
    //   .def(py::init<int>()) // Constructor, as needed for initialization
      .def_property_readonly(
          "num_nodes",
          &SearchSummary::num_nodes
      ) // Read-only access to fields
      .def_property_readonly("time", &SearchSummary::time)
      .def("get_result_depth_counts", &SearchSummary::GetResultDepthCounts)
      .def("get_transposition_table_hits", &SearchSummary::GetTranspositionTableHits)
      .def_property_readonly("similar_moves", &SearchSummary::similar_moves)
      .def_property_readonly("selected_move", &SearchSummary::selected_move)
      .def_property_readonly("returned_illegal_move", &SearchSummary::returned_illegal_move)
      .def_property_readonly("tr_table_size_initial", &SearchSummary::tr_table_size_initial)
      .def_property_readonly("tr_table_size_final", &SearchSummary::tr_table_size_final);

  py::class_<SearchSummaries>(m, "SearchSummaries")
      .def(py::init<>()) // Constructor, as needed for initialization
      .def_readonly(
          "first_searches",
          &SearchSummaries::first_searches
      ) // Read-only vectors and maps
      .def_readonly("extra_searches", &SearchSummaries::extra_searches);

  bind_minimax_move_evaluator<uint32_t>(m, "MinimaxMoveEvaluator32");
  bind_minimax_move_evaluator<uint64_t>(m, "MinimaxMoveEvaluator64");
  bind_minimax_move_evaluator<__uint128_t>(m, "MinimaxMoveEvaluator128");
  bind_zobrist_keys<uint32_t>(m, "ZobristKeys32");
  bind_zobrist_keys<uint64_t>(m, "ZobristKeys64");
  bind_zobrist_keys<__uint128_t>(m, "ZobristKeys128");
}
