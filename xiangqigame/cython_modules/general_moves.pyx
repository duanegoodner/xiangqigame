from libc.stdlib cimport labs
from libc.math cimport copysign


# mimic NamedTuple definition as cdef enum so search_spaces can be pure C
cdef enum PColor:
    RED = -1
    BLK = 1


cdef enum PType:
    NUL = 0
    GEN = 1
    ADV = 2
    ELE = 3
    HOR = 4
    CHA = 5
    CAN = 6
    SOL = 7


cdef long get_color(
        const long rank,
        const long file,
        const long [:, ::1] board_map):

    cdef long piece_val = board_map[rank, file]
    return 0 if piece_val == 0 else <long>copysign(1, piece_val)


cdef void get_castle_edges(
        const long color,
        long *castle_edges):
    if color == PColor.RED:
        castle_edges[0] = 7
        castle_edges[1] = 9
        castle_edges[2] = 3
        castle_edges[3] = 5
    elif color == PColor.BLK:
        castle_edges[0] = 0
        castle_edges[1] = 2
        castle_edges[2] = 3
        castle_edges[3] = 5


cdef long get_general_position(
        const long color,
        const long [:, ::1] board_map,
        long* general_position
):

    cdef long[4] castle
    get_castle_edges(color, castle)

    cdef Py_ssize_t cur_rank
    cdef Py_ssize_t cur_file
    cdef long general_found = 0

    for cur_rank in range(castle[0], castle[1] + 1):
        for cur_file in range(castle[2], castle[3] + 1):
            if labs(board_map[cur_rank, cur_file]) == PType.GEN:
                general_position[0] = cur_rank
                general_position[1] = cur_file
                general_found = 1

    return general_found


def run_get_general_position(const long color, const long [:, ::1] board_map):

    cdef long[2] general_position
    cdef long general_found = get_general_position(
        color=color, board_map=board_map, general_position = general_position
    )

    if general_found:
        return general_position[0], general_position[1]


cdef long flying_general_move(
        const long from_rank,
        const long from_file,
        const long color,
        const long [:, ::1] board_map,
        long* flying_move):

    cdef long has_flying_move = 1
    cdef long[2] other_gen_position = [-1, -1]

    cdef long opponent_color = -1 * color

    cdef long other_gen_position_found

    other_gen_position_found = get_general_position(
        color=opponent_color,
        board_map=board_map,
        general_position=other_gen_position)

    cdef Py_ssize_t slice_start
    cdef Py_ssize_t slice_end
    cdef Py_ssize_t rank

    if from_file != other_gen_position[1]:
        has_flying_move = 0
    else:
        slice_start = min(from_rank, other_gen_position[0]) + 1
        slice_end = max(from_rank, other_gen_position[0])
        for rank in range(slice_start, slice_end):
            if board_map[rank, from_file] != 0:
                has_flying_move = 0
                break
        if has_flying_move:
            flying_move[0] = from_rank
            flying_move[1] = from_file
            flying_move[2] = other_gen_position[0]
            flying_move[3] = other_gen_position[1]

    return has_flying_move


def run_flying_general_moves(
        const long from_rank,
        const long from_file,
        const long color,
        const long [:, ::1] board_map):
    cdef long[4] flying_move = [-1, -1, -1, -1]

    has_flying_move = flying_general_move(
        from_rank=from_rank, from_file=from_file, color=color,
        board_map=board_map, flying_move=flying_move)

    if not has_flying_move:
        return []
    else:
        return [{
            "start": (flying_move[0], flying_move[1]),
            "end": (flying_move[2], flying_move[1])
        }]


cdef is_in_castle(
        const long rank,
        const long file,
        const long[4] castle):

    return (castle[0] < rank < castle[1]) and (castle[2] < file < castle[3])


cdef long standard_general_moves(
        const long from_rank,
        const long from_file,
        const long color,
        const long [:, ::1] board_map,
        long* dest_coords):

    cdef long[8] orthog_directions = [
        0, 1,
        0, -1,
        1, 0,
        -1, 0
    ]

    cdef long[8] adjacent_spaces = [-1, -1, -1, -1, -1, -1, -1, -1]
    cdef long num_destinations = 0

    cdef long[4] castle
    get_castle_edges(color, castle)

    cdef long direction
    cdef long row
    cdef long col

    for direction in range(4):

        rank_idx = 2 * direction
        file_idx = 2 * direction + 1

        adjacent_spaces[rank_idx] = (
                from_rank + orthog_directions[rank_idx])
        adjacent_spaces[file_idx] = (
            from_file + orthog_directions[file_idx])

        if (castle[0] < adjacent_spaces[rank_idx] < castle[1]) and (
            castle[2] < adjacent_spaces[file_idx] < castle[3]) and (
                get_color(
            rank=adjacent_spaces[rank_idx],
            file=adjacent_spaces[file_idx],
            board_map=board_map) != color):
                dest_coords[2 * num_destinations] = adjacent_spaces[rank_idx]
                dest_coords[2 * num_destinations + 1] = adjacent_spaces[file_idx]
                num_destinations += 1


    return num_destinations


def run_standard_general_moves(
        const long from_rank,
        const long from_file,
        const long color,
        const long [:, ::1] board_map):

    cdef long[8] dest_coords
    cdef long num_dests = standard_general_moves(
        from_rank=from_rank,
        from_file=from_file,
        color=color,
        board_map=board_map,
        dest_coords=dest_coords)

    return [{
        "start": (from_rank, from_file),
        "end": (dest_coords[item], dest_coords[item + 1])
    } for item in range(0, (2 * num_dests)) if (item % 2 == 0)]




