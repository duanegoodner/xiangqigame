import numpy as np
import pytest

import xiangqigame.game_board_new as gbn


@pytest.fixture
def np_board():
    return gbn.GameBoard()


def test_is_occupied(np_board):
    assert np_board.is_occupied(space=np.array([0, 0]))
    assert not np_board.is_occupied(space=np.array([1, 0]))


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
    my_slice_a = np_board.get_slice(
        from_space=np.array([2, 2]),
        direction=np.array([1, 0]))
    expected_a = np.array([
        [3, 2, 7],
        [4, 2, 0],
        [5, 2, 0],
        [6, 2, -7],
        [7, 2, 0],
        [8, 2, 0],
        [9, 2, -3]])

    assert (my_slice_a == expected_a).all()

    my_slice_b = np_board.get_slice(
        from_space=np.array([6, 3]), direction=np.array([0, 1]))
    expected_b = np.array([
        [6, 4, -7],
        [6, 5, 0],
        [6, 6, -7],
        [6, 7, 0],
        [6, 8, -7]
    ])

    assert (my_slice_b == expected_b).all()


def test_execute_undo_move(np_board):
    move_a = np_board.execute_move(move=np.array([6, 2, 5, 2]))
    assert np_board._map[5, 2] == gbn.PieceColor.RED.value * gbn.PieceType.SOL.value
    assert np_board._map[6, 2] == gbn.PieceType.NUL.value

    np_board.undo_move(move_a)
    assert np_board._map[6, 2] == gbn.PieceColor.RED.value * gbn.PieceType.SOL.value
    assert np_board._map[5, 2] == gbn.PieceType.NUL.value


def test_get_all_spaces_occupied_by(np_board):
    red_spaces = np_board.get_all_spaces_occupied_by(gbn.PieceColor.RED)
    assert (red_spaces == np.array([
        [6, 0], [6, 2], [6, 4], [6, 6], [6, 8],
        [7, 1], [7, 7],
        [9, 0], [9, 1], [9, 2], [9, 3], [9, 4], [9, 5], [9, 6], [9, 7], [9, 8]
    ])).all()


def test_get_general_position(np_board):
    red_gen_position = np_board.get_general_position(gbn.PieceColor.RED)
    assert (red_gen_position == np.array([9, 4])).all()
    black_gen_position = np_board.get_general_position(gbn.PieceColor.BLK)
    assert (black_gen_position == np.array([0, 4])).all()


def test_is_in_homeland_of(np_board):
    assert np_board.is_in_homeland_of(gbn.PieceColor.RED, np.array([8, 2]))
    assert np_board.is_in_homeland_of(gbn.PieceColor.BLK, np.array([1, 4]))
    assert not np_board.is_in_homeland_of(gbn.PieceColor.RED, np.array([1, 5]))
    assert not np_board.is_in_homeland_of(gbn.PieceColor.BLK, np.array([9, 6]))

