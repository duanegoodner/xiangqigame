import json

import pkgutil
import pytest

from xiangqigame.game_board import GameBoard


@pytest.fixture
def starting_game_board() -> GameBoard:
    game_config = json.loads(pkgutil.get_data('xiangqigame.data', 'game_start.json'))
    return GameBoard(game_config['board_data'])






