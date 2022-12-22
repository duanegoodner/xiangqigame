#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "move_selector.hpp"
#include "board_components.hpp"

namespace py = pybind11;
using namespace py::literals;
// using namespace board_components;

PYBIND11_MODULE(MoveSelectorPy, m) {
    py::class_<PiecePointsMoveSelector>(m, "PiecePointsMoveSelector")
        .def(py::init<int>(), "search_depth"_a)
        .def(
            "SelectMove",
            &PiecePointsMoveSelector::SelectMove);
   
}