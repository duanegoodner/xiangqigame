#include <game_board.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <board_components.hpp>
#include <hash_calculator.hpp>
#include <minimax_evaluator.hpp>
#include <move_selector.hpp>
#include <piece_points.hpp>

namespace py = pybind11;
using namespace py::literals;
using namespace piece_points;
// using namespace board_components;

class PiecePointsMinimaxMoveSelector
    : public MinimaxMoveSelector<
          PiecePointsEvaluator<GameBoard<HashCalculator>, PiecePoints>> {};

PYBIND11_MODULE(MoveSelectorPy, m) {
  py::class_<PiecePointsMinimaxMoveSelector>(
      m,
      "PiecePointsMinimaxMoveSelector"
  )
      .def(py::init<int>(), "search_depth"_a)
      .def(
          "select_move",
          &PiecePointsMinimaxMoveSelector::SelectMove,
          "game_board"_a,
          "piece_color"_a
      );

  py::class_<RandomMoveSelector<GameBoard<HashCalculator>>>(
      m,
      "RandomMoveSelector"
  )
      .def(py::init<>())
      .def(
          "select_move",
          &RandomMoveSelector<GameBoard<HashCalculator>>::SelectMove,
          "game_board"_a,
          "piece_color"_a
      );
}
