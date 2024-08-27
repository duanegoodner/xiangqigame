// Filename: move_selector_py.cpp
// Author: Duane Goodner
// Created: 2022-12-07
// Last Modified: 2024-08-16

// Description:
// Implements pybind module that exposes concrete MoveSelector classes to
// Python.

#include <game_board.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <board_components.hpp>
#include <hash_calculator.hpp>
#include <move_evaluators.hpp>
// #include <move_selector.hpp>
#include <piece_points.hpp>

namespace py = pybind11;
using namespace py::literals;
using namespace piece_points;
// using namespace board_components;

// class MinimaxMoveSelectorPy
//     : public MinimaxMoveSelector<
//           PiecePointsEvaluator<GameBoard<HashCalculator>, PiecePoints>> {};

class MinimaxMoveEvaluatorPy
    : public MoveEvaluatorInterface <
      MinimaxMoveEvaluator<NewGameBoard<HashCalculator>, PiecePoints>> {};

PYBIND11_MODULE(MoveSelectorPy, m) {
  py::class_<MinimaxMoveEvaluatorPy>(m, "MinimaxMoveSelectorPy")
      .def(
          py::init<PieceColor, int, NewGameBoard<HashCalculator>>(),
          "evaluating_player"_a,
          "search_depth"_a,
          "game_board"_a
      )
      .def("select_move", &MinimaxMoveEvaluatorPy::SelectMove);

  //   py::class_<RandomMoveSelector<GameBoard<HashCalculator>>>(
  //       m,
  //       "RandomMoveSelector"
  //   )
  //       .def(py::init<>())
  //       .def(
  //           "select_move",
  //           &RandomMoveSelector<GameBoard<HashCalculator>>::SelectMove,
  //           "game_board"_a,
  //           "piece_color"_a
  //       );
}
