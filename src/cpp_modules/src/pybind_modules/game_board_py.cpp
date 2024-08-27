// Filename: game_board_py.cpp
// Author: Duane Goodner
// Created: 2022-12-07
// Last Modified: 2024-08-16

// Description:
// Implements pybind module that exposes GameBoard and related classes /
// functions to Python.

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <board_components.hpp>
#include <game_board.hpp>
#include <hash_calculator.hpp>
#include <minimax_evaluator.hpp>
#include <piece_points.hpp>

namespace py = pybind11;
using namespace py::literals;
using namespace board_components;
using namespace piece_points;

// class MinimaxMoveEvaluatorPy
//     : public MoveEvaluatorInterface<
//           MinimaxMoveEvaluator<NewGameBoard<HashCalculator>, PiecePoints>>
//           {};

// class GameBoardPy : public SpaceInfoProvider<HashCalculator> {};

PYBIND11_MODULE(XiangqigamePy, m) {

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

  py::class_<NewGameBoard<HashCalculator>>(m, "GameBoard")
      .def(py::init<>())
      .def("map", &NewGameBoard<HashCalculator>::map)
      .def("ExecuteMove", &NewGameBoard<HashCalculator>::ExecuteMove, "move"_a)
      .def(
          "UndoMove",
          &NewGameBoard<HashCalculator>::UndoMove,
          "executed_move"_a
      )
      .def(
          "GetAllSpacesOccupiedBy",
          &NewGameBoard<HashCalculator>::GetAllSpacesOccupiedBy,
          "color"_a
      )
      .def(
          "CalcFinalMovesOf",
          &NewGameBoard<HashCalculator>::CalcFinalMovesOf,
          "color"_a
      )
      .def("IsInCheck", &NewGameBoard<HashCalculator>::IsInCheck, "color"_a)
      .def("GetType", &NewGameBoard<HashCalculator>::GetType, "space"_a)
      .def("GetColor", &NewGameBoard<HashCalculator>::GetColor, "space"_a);

  m.def("opponent_of", &opponent_of);

  py::class_<MinimaxMoveEvaluator<NewGameBoard<HashCalculator>, PiecePoints>>(m, "MinimaxMoveSelectorPy")
      .def(
          py::init<PieceColor, int, NewGameBoard<HashCalculator>&>(),
          "evaluating_player"_a,
          "search_depth"_a,
          "game_board"_a
      )
      .def("select_move", &MinimaxMoveEvaluator<NewGameBoard<HashCalculator>, PiecePoints>::SelectMove);
}
