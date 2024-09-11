// Filename: xiangqigame_core.cpp
// Author: Duane Goodner
// Created: 2022-12-07
// Last Modified: 2024-09-08

// Description:
// Implements pybind module that exposes GameBoard and related classes /
// functions to Python.

#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <board_components.hpp>
#include <game_board.hpp>
#include <hash_calculator.hpp>
#include <move_evaluators.hpp>
#include <piece_points.hpp>
#include <string>

namespace py = pybind11;
using namespace py::literals;
using namespace board_components;
using namespace piece_points;

template <typename KeyType>
void bind_minimax_move_evaluator(py::module_ &m, const std::string &class_name) {
  py::class_<MinimaxMoveEvaluator<NewGameBoard, HashCalculator<KeyType>, PiecePoints>>(
      m,
      class_name.c_str()
  )
      .def(
          py::init<PieceColor, int, NewGameBoard &>(),
          "evaluating_player"_a,
          "starting_search_depth"_a,
          "game_board"_a
      )
      .def(
          "select_move",
          &MinimaxMoveEvaluator<NewGameBoard, HashCalculator<KeyType>, PiecePoints>::
              SelectMove
      )
      .def(
          "get_search_summaries",
          &MinimaxMoveEvaluator<NewGameBoard, HashCalculator<KeyType>, PiecePoints>::
              GetSearchSummaries,
          py::return_value_policy::copy
      )
      .def(
          "zobrist_key_size_bits",
          &MinimaxMoveEvaluator<NewGameBoard, HashCalculator<KeyType>, PiecePoints>::
              KeySizeBits
      );
}

PYBIND11_MODULE(xiangqigame_core, m) {

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

  py::class_<NewGameBoard>(m, "GameBoard")
      .def(py::init<>())
      .def("map", &NewGameBoard::map)
      .def("ExecuteMove", &NewGameBoard::ExecuteMove, "move"_a)
      .def("UndoMove", &NewGameBoard::UndoMove, "executed_move"_a)
      .def("GetAllSpacesOccupiedBy", &NewGameBoard::GetAllSpacesOccupiedBy, "color"_a)
      .def("CalcFinalMovesOf", &NewGameBoard::CalcFinalMovesOf, "color"_a)
      .def("IsInCheck", &NewGameBoard::IsInCheck, "color"_a)
      .def("GetType", &NewGameBoard::GetType, "space"_a)
      .def("GetColor", &NewGameBoard::GetColor, "space"_a);

  m.def("opponent_of", &opponent_of);

  py::class_<RandomMoveEvaluator<NewGameBoard>>(m, "RandomMoveEvaluator")
      .def(py::init<PieceColor, NewGameBoard &>(), "evaluating_player"_a, "game_board"_a)
      .def("select_move", &RandomMoveEvaluator<NewGameBoard>::SelectMove);

  py::class_<SearchSummary>(m, "NewSearchSummary")
      .def(py::init<int>()) // Constructor, as needed for initialization
      .def_readonly(
          "num_nodes",
          &SearchSummary::num_nodes
      ) // Read-only access to fields
      .def_readonly("time", &SearchSummary::time)
      .def_readonly("result_depth_counts", &SearchSummary::result_depth_counts);

  py::class_<SearchSummaries>(m, "SearchSummaries")
      .def(py::init<>()) // Constructor, as needed for initialization
      .def_readonly(
          "first_searches",
          &SearchSummaries::first_searches
      ) // Read-only vectors and maps
      .def_readonly("extra_searches", &SearchSummaries::extra_searches);

  bind_minimax_move_evaluator<uint64_t>(m, "MinimaxMoveEvaluator64");
  bind_minimax_move_evaluator<__uint128_t>(m, "MinimaxMoveEvaluator128");
}
