from game_piece import GamePiece
from game_board import GameBoard


def chariot(board):
    return True


def horse(board):
    return True


def elephant(board):
    return True


def advisor(board):
    return True


def general(board: GameBoard, curr_position, curr_piece: GamePiece):

    allowed_moves = set()

    other_gen_position = board.get_general_position(curr_piece.get_opp_color())

    if curr_position[1] == other_gen_position[1]:
        intermediate_pieces = board.get_vertical_path(curr_position, other_gen_position[1])
        if all(piece.is_null_piece() for piece in intermediate_pieces):
            allowed_moves.add((curr_position, other_gen_position))

    return allowed_moves


def cannon(board):
    return True


def soldier(board):
    return True









