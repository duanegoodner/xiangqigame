from game_piece import GamePiece
from game_board import GameBoard


def in_same_rank(curr_position, dest_position):
    return curr_position[0] == dest_position[0]


def in_same_file(curr_position, dest_position):
    return curr_position[1] == dest_position[1]


def chariot(board, curr_position, curr_piece):
    return set()


def horse(board, curr_position, curr_piece):
    return set()


def elephant(board, curr_position, curr_piece):
    return set()


def advisor(board, curr_position, curr_piece):
    return set()


def flying_general(board: GameBoard, curr_position, curr_piece: GamePiece):
    flying_move = set()

    other_gen_position = board.get_general_position(curr_piece.get_opp_color())

    if in_same_file(curr_position, other_gen_position):
        intermediate_pieces = \
            board.get_vertical_path(curr_position, other_gen_position[1])
        if all(piece.is_null_piece() for piece in intermediate_pieces):
            flying_move.add((curr_position, other_gen_position))

    return flying_move


def standard_general(board: GameBoard, curr_position, curr_piece: GamePiece):
    return {(curr_position, space) for space in
            board.get_adjacent_spaces(curr_position) if space in
            board.get_castle(curr_piece.get_color())}


def general(board: GameBoard, curr_position, curr_piece: GamePiece):

    flying_move = flying_general(board, curr_position, curr_piece)
    standard_moves = standard_general(board, curr_position, curr_piece)

    return flying_move | standard_moves


def cannon(board, curr_position, curr_piece):
    return set()


def soldier(board, curr_position, curr_piece):
    return set()


_move_functions = {
    'chariot': chariot,
    'horse': horse,
    'elephant': elephant,
    'advisor': advisor,
    'general': general,
    'cannon': cannon,
    'soldier': soldier,
}


def get_move_function(piece_type):
    return _move_functions[piece_type]


# def resulting_board(board, move):
#
#
#
# def get_moves(board: GameBoard, color):
#
#     occ_map = board.get_all_pieces_of(color)













