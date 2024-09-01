import pytest

# from xiangqigame.board_utilities_new import BoardUtilities as bu
from xiangqigame_core import (
    BoardSpace,
    GameBoard,
    Move,
    PieceColor,
    PieceType,
)


@pytest.fixture
def cpp_board():
    return GameBoard()


def test_get_piece_type(cpp_board):
    assert cpp_board.GetType(BoardSpace(0, 4)) == PieceType.kGen
    assert cpp_board.GetType(BoardSpace(9, 0)) == PieceType.kCha
    assert cpp_board.GetType(BoardSpace(4, 1)) == PieceType.kNnn


def test_get_piece_color(cpp_board):
    assert cpp_board.GetColor(BoardSpace(0, 8)) == PieceColor.kBlk
    assert cpp_board.GetColor(BoardSpace(7, 7)) == PieceColor.kRed
    assert cpp_board.GetColor(BoardSpace(8, 5)) == PieceColor.kNul


def test_execute_undo_move(cpp_board):
    move_a = cpp_board.ExecuteMove(move=Move(BoardSpace(6, 2), BoardSpace(5, 2)))
    assert cpp_board.GetColor(BoardSpace(rank=5, file=2)) == PieceColor.kRed
    assert cpp_board.GetType(BoardSpace(rank=5, file=2)) == PieceType.kSol
    assert cpp_board.GetType(BoardSpace(rank=6, file=2)) == PieceType.kNnn

    cpp_board.UndoMove(move_a)
    assert cpp_board.GetColor(BoardSpace(rank=6, file=2)) == PieceColor.kRed
    assert cpp_board.GetType(BoardSpace(rank=6, file=2)) == PieceType.kSol
    assert cpp_board.GetType(BoardSpace(rank=5, file=2)) == PieceType.kNnn


def test_GetAllSpacesOccupiedBy(cpp_board):
    red_spaces = cpp_board.GetAllSpacesOccupiedBy(PieceColor.kRed)


def test_calc_final_moves(cpp_board):
    result = cpp_board.CalcFinalMovesOf(color=PieceColor.kRed)
    assert result.size() == 44
