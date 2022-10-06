import numpy as np
import pytest
import time

import xiangqigame.game_board_no_enums as gb_ne


@pytest.fixture
def no_enum_board():
    return gb_ne.GameBoard()


def test_board_builder_init():
    bb = gb_ne.StartingBoardBuilder()


def test_board_builder_build():
    bb = gb_ne.StartingBoardBuilder()
    board = bb.build()
    print(f"\n{board}")


def test_is_occupied(no_enum_board):
    assert no_enum_board.is_occupied(np.array([0, 4]))
    assert not no_enum_board.is_occupied(np.array([1, 0]))


def test_get_piece_type(no_enum_board):
    assert no_enum_board.get_piece_type(np.array([0, 4])) == gb_ne.PTYPE.GEN
    assert no_enum_board.get_piece_type(np.array([9, 0])) == gb_ne.PTYPE.CHA
    assert no_enum_board.get_piece_type(np.array([4, 1])) == gb_ne.PTYPE.NUL


def test_get_piece_color(no_enum_board):
    assert no_enum_board.get_piece_color(
        np.array([0, 8])) == gb_ne.PCOLOR.BLK
    assert no_enum_board.get_piece_color(
        np.array([7, 7])) == gb_ne.PCOLOR.RED
    assert no_enum_board.get_piece_color(
        np.array([8, 5])) == gb_ne.PCOLOR.NUL


def test_get_all_spaces_occupied_by(no_enum_board):
    no_enum_board.get_all_spaces_occupied_by(gb_ne.PCOLOR.RED)