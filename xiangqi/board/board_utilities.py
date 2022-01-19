from xiangqi.common.enums import PieceColor
from xiangqi.board.board_components import BoardSpace, BoardVector, river_edges

board_dim = {
    'num_files': 9,
    'num_ranks': 10
}

forward_direction = {
    PieceColor.RED: 1,
    PieceColor.BLACK: -1
}

board_vectors_horiz = [BoardVector(0, 1), BoardVector(0, -1)]
board_vectors_vert = [BoardVector(1, 0), BoardVector(-1, 0)]


horse_paths = {
    BoardVector(1, 0): (BoardVector(1, 1), BoardVector(1, -1)),
    BoardVector(-1, 0): (BoardVector(-1, 1), BoardVector(-1, -1)),
    BoardVector(0, 1): (BoardVector(1, 1), BoardVector(-1, 1)),
    BoardVector(0, -1): (BoardVector(1, -1), BoardVector(-1, -1))
}

diag_directions = [(BoardVector(rank, file)) for rank in [-1, 1] for file
                   in [-1, 1]]


def in_same_rank(curr_position: BoardSpace, dest_position: BoardSpace):
    return curr_position.rank == dest_position.rank


def in_same_file(curr_position: BoardSpace, dest_position: BoardSpace):
    return curr_position.file == dest_position.file


def is_on_board(space: BoardSpace):
    return (0 <= space.rank < board_dim['num_ranks']) and \
           (0 <= space.file < board_dim['num_files'])


def is_in_homeland_of(piece_color: PieceColor, board_space: BoardSpace):
    return forward_direction[piece_color] * board_space.rank <=\
           forward_direction[piece_color] * river_edges[piece_color]


def get_adjacent_spaces(board_space: BoardSpace, horizontal: bool = True,
                        vertical: bool = True):
    unit_vectors = []
    if horizontal:
        unit_vectors += board_vectors_horiz
    if vertical:
        unit_vectors += board_vectors_vert

    return {board_space.add_board_vector(unit_vector) for unit_vector
            in unit_vectors if
            is_on_board(board_space.add_board_vector(unit_vector))}
