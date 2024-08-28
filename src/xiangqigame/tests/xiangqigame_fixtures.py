from typing import Dict

import pytest
from cpp_modules.src.pybind_modules.GameBoardPy import GameBoard


@pytest.fixture
def starting_game_board() -> GameBoard:
    return GameBoard()
