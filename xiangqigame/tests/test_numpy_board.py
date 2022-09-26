import numpy as np
import pytest

import xiangqigame.game_board_new as gbn


@pytest.fixture
def np_board():
    return gbn.GameBoard()


def test_is_occupied(np_board):
    assert np_board.is_occupied(np.array([0, 0]))
    assert not np_board.is_occupied(np.array([1, 0]))


def test_get_piece_type(np_board):
    assert np_board.get_piece_type(np.array([0, 4])) == gbn.PieceType.GEN
    assert np_board.get_piece_type(np.array([9, 0])) == gbn.PieceType.CHA
    assert np_board.get_piece_type(np.array([4, 1])) == gbn.PieceType.NUL


def test_get_piece_color(np_board):
    assert np_board.get_piece_color(np.array([0, 8])) == gbn.PieceColor.BLK
    assert np_board.get_piece_color(np.array([7, 7])) == gbn.PieceColor.RED
    assert np_board.get_piece_color(np.array([8, 5])) == gbn.PieceColor.NUL


def test_forward_direction(np_board):
    assert (np_board.forward_direction(
                gbn.PieceColor.RED) == np.array([-1, 0])).all()
    assert (np_board.forward_direction(
        gbn.PieceColor.BLK) == np.array([1, 0])).all()
    assert (np_board.forward_direction(
                gbn.PieceColor.NUL) == np.array([0, 0])).all()


def test_get_slice(np_board):
    my_slice_a = np_board.get_slice(from_space=np.array([2, 2]), direction=np.array([1, 0]))
    expected_a = np.array([7, 0, 0, -7, 0, 0, -3])

    assert (my_slice_a == expected_a).all()

    my_slice_b = np_board.get_slice(from_space=np.array([6, 3]), direction=np.array([0, 1]))
    expected_b = np.array([-7, 0, -7, 0, -7])

    assert (my_slice_b == expected_b).all()
