import pytest
from xiangqigame.board_utilities_new import BoardUtilities as bu
# import xiangqigame.game_board_new as gbn
# from xiangqigame.piece_definitions import PColor

import cpp_modules.game_board_py.GameBoardPy as gbp
# from cpp_modules.game_board_py import BoardSpace, GameBoard, PieceColor, PieceType


@pytest.fixture
def cpp_board():
    return gbp.GameBoard()


# def test_IsOccupied(cpp_board):
#     assert cpp_board.IsOccupied((0, 0))
#     assert not cpp_board.IsOccupied((1, 0))


def test_get_piece_type(cpp_board):
    assert cpp_board.GetType(gbp.BoardSpace(0, 4)) == gbp.PieceType.kGen
    assert cpp_board.GetType(gbp.BoardSpace(9, 0)) == gbp.PieceType.kCha
    assert cpp_board.GetType(gbp.BoardSpace(4, 1)) == gbp.PieceType.kNnn


def test_get_piece_color(cpp_board):
    assert cpp_board.GetColor(gbp.BoardSpace(0, 8)) == gbp.PieceColor.kBlk
    assert cpp_board.GetColor(gbp.BoardSpace(7, 7)) == gbp.PieceColor.kRed
    assert cpp_board.GetColor(gbp.BoardSpace(8, 5)) == gbp.PieceColor.kNul


def test_forward_direction(cpp_board):
    assert bu.fwd_unit_vect(gbp.PieceColor.kRed) == (-1, 0)
    assert bu.fwd_unit_vect(gbp.PieceColor.kBlk) == (1, 0)
    assert bu.fwd_unit_vect(gbp.PieceColor.kNul) == (0, 0)


def test_execute_undo_move(cpp_board):
    move_a = cpp_board.ExecuteMove(
        move=gbp.Move(gbp.BoardSpace(6, 2), gbp.BoardSpace(5, 2)))
    assert cpp_board.map()[5][2] == int(gbp.PieceColor.kRed) * int(gbp.PieceType.kSol)
    assert cpp_board.map()[6][2] == gbp.PieceType.kNnn

    cpp_board.UndoMove(move_a)
    assert cpp_board.map()[6][2] == int(gbp.PieceColor.kRed) * int(gbp.PieceType.kSol)
    assert cpp_board.map()[5][2] == gbp.PieceType.kNnn


def test_GetAllSpacesOccupiedBy(cpp_board):
    red_spaces = cpp_board.GetAllSpacesOccupiedBy(gbp.PieceColor.kRed)


def test_is_in_homeland_of(cpp_board):
    assert bu.is_in_homeland_of(gbp.PieceColor.kRed, (8, 2))
    assert bu.is_in_homeland_of(gbp.PieceColor.kBlk, (1, 4))
    assert not bu.is_in_homeland_of(gbp.PieceColor.kRed, (1, 5))
    assert not bu.is_in_homeland_of(gbp.PieceColor.kBlk, (9, 6))


def test_calc_final_moves(cpp_board):
    result = cpp_board.CalcFinalMovesOf(color=gbp.PieceColor.kRed)
    assert len(result) == 44


# def test_get_general_moves(cpp_board):
#     red_gen_moves = cpp_board.general_moves(from_position=(9, 4), color=gbp.PieceColor.kRed)
#     black_cannon_moves = cpp_board.cannon_moves(from_position=(2, 1), color=gbp.PieceColor.kBlk)
