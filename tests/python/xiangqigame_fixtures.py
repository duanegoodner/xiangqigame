from typing import Dict

import pytest
from xiangqigame_core import GameBoard


@pytest.fixture
def starting_game_board() -> GameBoard:
    return GameBoard()
