import xiangqi_bindings as bindings


def test_game_board_factory_instantiation():
    game_board_factory = bindings.GameBoardFactory()

def test_game_board_creation():
    game_board_new = bindings.GameBoardFactory().create()