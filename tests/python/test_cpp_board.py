import pytest
# from xiangqigame.board_utilities_new import BoardUtilities as bu
from cpp_modules.src.pybind_modules.GameBoardPy import BoardSpace, GameBoard, Move, PieceColor, PieceType


@pytest.fixture
def cpp_board():
    return GameBoard()


# def test_IsOccupied(cpp_board):
#     assert cpp_board.IsOccupied((0, 0))
#     assert not cpp_board.IsOccupied((1, 0))


def test_get_piece_type(cpp_board):
    assert cpp_board.GetType(BoardSpace(0, 4)) == PieceType.kGen
    assert cpp_board.GetType(BoardSpace(9, 0)) == PieceType.kCha
    assert cpp_board.GetType(BoardSpace(4, 1)) == PieceType.kNnn


def test_get_piece_color(cpp_board):
    assert cpp_board.GetColor(BoardSpace(0, 8)) == PieceColor.kBlk
    assert cpp_board.GetColor(BoardSpace(7, 7)) == PieceColor.kRed
    assert cpp_board.GetColor(BoardSpace(8, 5)) == PieceColor.kNul


# def test_forward_direction(cpp_board):
#     assert bu.fwd_unit_vect(PieceColor.kRed) == (-1, 0)
#     assert bu.fwd_unit_vect(PieceColor.kBlk) == (1, 0)
#     assert bu.fwd_unit_vect(PieceColor.kNul) == (0, 0)


def test_execute_undo_move(cpp_board):
    move_a = cpp_board.ExecuteMove(
        move=Move(BoardSpace(6, 2), BoardSpace(5, 2)))
    assert cpp_board.map()[5][2] == int(PieceColor.kRed) * int(PieceType.kSol)
    assert cpp_board.map()[6][2] == PieceType.kNnn

    cpp_board.UndoMove(move_a)
    assert cpp_board.map()[6][2] == int(PieceColor.kRed) * int(PieceType.kSol)
    assert cpp_board.map()[5][2] == PieceType.kNnn


def test_GetAllSpacesOccupiedBy(cpp_board):
    red_spaces = cpp_board.GetAllSpacesOccupiedBy(PieceColor.kRed)


# def test_is_in_homeland_of(cpp_board):
#     assert bu.is_in_homeland_of(PieceColor.kRed, (8, 2))
#     assert bu.is_in_homeland_of(PieceColor.kBlk, (1, 4))
#     assert not bu.is_in_homeland_of(PieceColor.kRed, (1, 5))
#     assert not bu.is_in_homeland_of(PieceColor.kBlk, (9, 6))


def test_calc_final_moves(cpp_board):
    result = cpp_board.CalcFinalMovesOf(color=PieceColor.kRed)
    assert len(result) == 44


# def test_get_general_moves(cpp_board):
#     red_gen_moves = cpp_board.general_moves(from_position=(9, 4), color=PieceColor.kRed)
#     black_cannon_moves = cpp_board.cannon_moves(from_position=(2, 1), color=PieceColor.kBlk)