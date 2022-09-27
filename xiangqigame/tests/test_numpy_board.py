import numpy as np
import pytest

import xiangqigame.game_board_new as gbn


@pytest.fixture
def np_board():
    return gbn.GameBoard()


def test_is_occupied(np_board):
    assert np_board.is_occupied((0, 0))
    assert not np_board.is_occupied((1, 0))


def test_get_piece_type(np_board):
    assert np_board.get_piece_type((0, 4)) == gbn.PieceType.GEN
    assert np_board.get_piece_type((9, 0)) == gbn.PieceType.CHA
    assert np_board.get_piece_type((4, 1)) == gbn.PieceType.NUL


def test_get_piece_color(np_board):
    assert np_board.get_piece_color((0, 8)) == gbn.PieceColor.BLK
    assert np_board.get_piece_color((7, 7)) == gbn.PieceColor.RED
    assert np_board.get_piece_color((8, 5)) == gbn.PieceColor.NUL


def test_forward_direction(np_board):
    assert (np_board.forward_direction(
                gbn.PieceColor.RED) == np.array([-1, 0])).all()
    assert (np_board.forward_direction(
        gbn.PieceColor.BLK) == np.array([1, 0])).all()
    assert (np_board.forward_direction(
                gbn.PieceColor.NUL) == np.array([0, 0])).all()


def test_get_slice(np_board):
    my_slice_a = np_board.get_slice(from_space=(2, 2), direction=(1, 0))
    expected_a = np.array([7, 0, 0, -7, 0, 0, -3])

    assert (my_slice_a == expected_a).all()

    my_slice_b = np_board.get_slice(from_space=(6, 3), direction=(0, 1))
    expected_b = np.array([-7, 0, -7, 0, -7])

    assert (my_slice_b == expected_b).all()


def test_execute_undo_move(np_board):
    move_a = np_board.execute_move(start=(6, 2), end=(5, 2))
    assert np_board._map[5, 2] == gbn.PieceColor.RED.value * gbn.PieceType.SOL.value
    assert np_board._map[6, 2] == gbn.PieceType.NUL.value

    np_board.undo_move(move_a)
    assert np_board._map[6, 2] == gbn.PieceColor.RED.value * gbn.PieceType.SOL.value
    assert np_board._map[5, 2] == gbn.PieceType.NUL.value
