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


def test_board_builder_build_tuple():
    bb = gb_ne.StartingBoardBuilder()
    board = bb.build_td()
    print(f"\n{board}")


def test_board_builder_build_ct():
    bb = gb_ne.StartingBoardBuilder()
    board = bb.build_ct()
    print(f"\n{board}")


