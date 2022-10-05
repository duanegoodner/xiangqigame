import pytest
from xiangqigame.board_components_new import BoardVector
from xiangqigame.board_utilities_new import BoardUtilities as bu
import xiangqigame.game_board_new as gbn
from xiangqigame.piece_definitions import PColor


@pytest.fixture
def np_board():
    return gbn.GameBoard()


def test_is_occupied(np_board):
    assert np_board.is_occupied((0, 0))
    assert not np_board.is_occupied((1, 0))


def test_get_piece_type(np_board):
    assert np_board.get_type((0, 4)) == gbn.PType.GEN
    assert np_board.get_type((9, 0)) == gbn.PType.CHA
    assert np_board.get_type((4, 1)) == gbn.PType.NUL


def test_get_piece_color(np_board):
    assert np_board.get_color((0, 8)) == gbn.PColor.BLK
    assert np_board.get_color((7, 7)) == gbn.PColor.RED
    assert np_board.get_color((8, 5)) == gbn.PColor.NUL


def test_forward_direction(np_board):
    assert bu.fwd_unit_vect(gbn.PColor.RED) == (-1, 0)
    assert bu.fwd_unit_vect(gbn.PColor.BLK) == (1, 0)
    assert bu.fwd_unit_vect(gbn.PColor.NUL) == (0, 0)


def test_get_general_position(np_board):
    red_gen_position = np_board.get_general_position(gbn.PColor.RED)
    assert red_gen_position == (9, 4)
    black_gen_position = np_board.get_general_position(gbn.PColor.BLK)
    assert black_gen_position == (0, 4)


def test_execute_undo_move(np_board):
    move_a = np_board.execute_move(
        {"start": (6, 2), "end": (5, 2)})
    assert np_board._map[5, 2] == gbn.PColor.RED * gbn.PType.SOL
    assert np_board._map[6, 2] == gbn.PType.NUL

    np_board.undo_move(move_a)
    assert np_board._map[6, 2] == gbn.PColor.RED * gbn.PType.SOL
    assert np_board._map[5, 2] == gbn.PType.NUL


def test_get_all_spaces_occupied_by(np_board):
    red_spaces = np_board.get_all_spaces_occupied_by(gbn.PColor.RED)


def test_is_in_homeland_of(np_board):
    assert bu.is_in_homeland_of(gbn.PColor.RED, (8, 2))
    assert bu.is_in_homeland_of(gbn.PColor.BLK, (1, 4))
    assert not bu.is_in_homeland_of(gbn.PColor.RED, (1, 5))
    assert not bu.is_in_homeland_of(gbn.PColor.BLK, (9, 6))


def test_calc_final_moves(np_board):
    result = np_board.calc_final_moves_of(color=PColor.RED)
    assert len(result) == 44



