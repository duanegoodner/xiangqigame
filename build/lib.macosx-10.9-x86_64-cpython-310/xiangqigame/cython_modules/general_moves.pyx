from .cython_board_utilities cimport get_color, get_castle_edges
from .cython_board_utilities cimport get_general_position, is_in_castle


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

        if is_in_castle(
                rank=adjacent_spaces[rank_idx],
                file=adjacent_spaces[file_idx],
                castle=castle) and (
                get_color(
            rank=adjacent_spaces[rank_idx],
            file=adjacent_spaces[file_idx],
            board_map=board_map) != color):
                dest_coords[2 * num_destinations] = adjacent_spaces[rank_idx]
                dest_coords[2 * num_destinations + 1] = adjacent_spaces[file_idx]
                num_destinations += 1


    return num_destinations


def run_general_moves(
        const long from_rank,
        const long from_file,
        const long [:, ::1] board_map,
        const long color):

    cdef long[4] flying_move = [-1, -1, -1, -1]
    has_flying_move = flying_general_move(
        from_rank=from_rank, from_file=from_file, color=color,
        board_map=board_map, flying_move=flying_move)

    if not has_flying_move:
        general_moves = []
    else:
        general_moves =  [{
            "start": (flying_move[0], flying_move[1]),
            "end": (flying_move[2], flying_move[1])
        }]

    cdef long[8] standard_dest_coords
    cdef long num_standard_dests = standard_general_moves(
        from_rank=from_rank,
        from_file=from_file,
        color=color,
        board_map=board_map,
        dest_coords=standard_dest_coords
    )

    general_moves += [{
        "start": (from_rank, from_file),
        "end": (standard_dest_coords[item], standard_dest_coords[item + 1])
    } for item in range(0, (2 * num_standard_dests)) if (item % 2 == 0)]

    return general_moves













