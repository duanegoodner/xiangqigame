#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <board_components.hpp>
#include <game_board.hpp>
#include <hash_calculator.hpp>
#include <piece_points.hpp>

namespace py = pybind11;
using namespace py::literals;
using namespace board_components;

PYBIND11_MODULE(GameBoardPy, m) {
  py::class_<BoardSpace>(m, "BoardSpace")
      .def(py::init<int, int>(), "rank"_a, "file"_a)
      .def_readonly("rank", &BoardSpace::rank)
      .def_readonly("file", &BoardSpace::file);

  py::class_<Move>(m, "Move")
      .def(py::init<BoardSpace, BoardSpace>(), "start"_a, "end"_a)
      .def_readonly("start", &Move::start)
      .def_readonly("end", &Move::end);

  py::class_<MoveCollection>(m, "MoveCollection")
      .def_readonly("moves", &MoveCollection::moves)
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

  py::class_<GameBoard<HashCalculator>>(m, "GameBoard")
      .def(py::init<>())
      .def("map", &GameBoard<HashCalculator>::map)
      .def("ExecuteMove", &GameBoard<HashCalculator>::ExecuteMove, "move"_a)
      .def("UndoMove", &GameBoard<HashCalculator>::UndoMove, "executed_move"_a)
      .def(
          "GetAllSpacesOccupiedBy",
          &GameBoard<HashCalculator>::GetAllSpacesOccupiedBy,
          "color"_a
      )
      .def("CalcFinalMovesOf", &GameBoard<HashCalculator>::CalcFinalMovesOf, "color"_a)
      .def("IsInCheck", &GameBoard<HashCalculator>::IsInCheck, "color"_a)
      .def("GetType", &GameBoard<HashCalculator>::GetType, "space"_a)
      .def("GetColor", &GameBoard<HashCalculator>::GetColor, "space"_a);
}
