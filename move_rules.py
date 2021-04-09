from typing import Dict

from game_piece import GamePiece
from game_board import GameBoard


_opponent_of = {
    'RED': 'BLACK',
    'BLACK': 'RED'
}


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


_move_functions: Dict[str, any] = {
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


def get_occupancy_map(board: GameBoard, color):

    return {(row, col): board.get_occupant((row, col)) for col in
            range(board.get_num_files()) for row in
            range(board.get_num_ranks()) if
            board.get_occupant((row, col)).get_color() == color}


def get_moves_it(board, space, piece):
    """
    Returns a set of (possible) moves for piece, ignoring possibility that
    destination may be occupied by teammate (it = Ignore Teammates)
    """
    # Look up correct move function in _move_functions dictionary (based on
    # piece_type), and call that function
    return _move_functions[piece.get_piece_type()](board, space, piece)


def get_untested_moves(board: GameBoard, color):
    moves = set()
    occ_map = get_occupancy_map(board, color)

    for space, piece in occ_map.items():
        for (from_space, to_space) in get_moves_it(board, space, piece):
            if to_space not in occ_map:
                moves.add((from_space, to_space))

    return moves


def is_checked(board: GameBoard, color):
    gen_position = board.get_general_position(color)

    for from_space, to_space in get_untested_moves(board, _opponent_of[color]):
        if to_space == gen_position:
            return True

    return False



# def resulting_board(board, move):
#
#
#
# def get_moves(board: GameBoard, color):
#
#     occ_map = board.get_all_pieces_of(color)













