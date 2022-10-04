import pytest
from xiangqigame.board_components_new import BoardVector
from xiangqigame.board_utilities_new import BoardUtilities as bu
import xiangqigame.game_board_new as gbn
from xiangqigame.piece_definitions import PColor


@pytest.fixture
def np_board():
    return gbn.GameBoard()


def test_is_occupied(np_board):
    assert np_board.is_occupied(gbn.BoardSpace(rank=0, file=0))
    assert not np_board.is_occupied(gbn.BoardSpace(rank=1, file=0))


def test_get_piece_type(np_board):
    assert np_board.get_type(gbn.BoardSpace(rank=0, file=4)) == gbn.PType.GEN
    assert np_board.get_type(gbn.BoardSpace(rank=9, file=0)) == gbn.PType.CHA
    assert np_board.get_type(gbn.BoardSpace(rank=4, file=1)) == gbn.PType.NUL


def test_get_piece_color(np_board):
    assert np_board.get_color(gbn.BoardSpace(rank=0, file=8)) == gbn.PColor.BLK
    assert np_board.get_color(gbn.BoardSpace(rank=7, file=7)) == gbn.PColor.RED
    assert np_board.get_color(gbn.BoardSpace(rank=8, file=5)) == gbn.PColor.NUL


def test_forward_direction(np_board):
    assert bu.fwd_unit_vect(gbn.PColor.RED) == BoardVector(rank=-1, file=0)
    assert bu.fwd_unit_vect(gbn.PColor.BLK) == BoardVector(rank=1, file=0)
    assert bu.fwd_unit_vect(gbn.PColor.NUL) == BoardVector(rank=0, file=0)


def test_execute_undo_move(np_board):
    move_a = np_board.execute_move(
        gbn.Move(start=gbn.BoardSpace(rank=6, file=2),
                 end=gbn.BoardSpace(rank=5, file=2)))
    assert np_board._map[5, 2] == gbn.PColor.RED * gbn.PType.SOL
    assert np_board._map[6, 2] == gbn.PType.NUL

    np_board.undo_move(move_a)
    assert np_board._map[6, 2] == gbn.PColor.RED * gbn.PType.SOL
    assert np_board._map[5, 2] == gbn.PType.NUL


def test_get_all_spaces_occupied_by(np_board):
    red_spaces = np_board.get_all_spaces_occupied_by(gbn.PColor.RED)


def test_get_general_position(np_board):
    red_gen_position = np_board.get_general_position(gbn.PColor.RED)
    assert red_gen_position == gbn.BoardSpace(rank=9, file=4)
    black_gen_position = np_board.get_general_position(gbn.PColor.BLK)
    assert black_gen_position == gbn.BoardSpace(rank=0, file=4)


def test_is_in_homeland_of(np_board):
    assert bu.is_in_homeland_of(gbn.PColor.RED, gbn.BoardSpace(rank=8, file=2))
    assert bu.is_in_homeland_of(gbn.PColor.BLK, gbn.BoardSpace(rank=1, file=4))
    assert not bu.is_in_homeland_of(
        gbn.PColor.RED, gbn.BoardSpace(rank=1, file=5))
    assert not bu.is_in_homeland_of(
        gbn.PColor.BLK, gbn.BoardSpace(rank=9, file=6))


def test_calc_final_moves(np_board):
    result = np_board.calc_final_moves_of(color=PColor.RED)
    assert len(result) == 44



