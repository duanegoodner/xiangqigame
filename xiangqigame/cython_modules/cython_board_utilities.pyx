import cython
from libc.stdlib cimport labs
from libc.math cimport copysign


@cython.boundscheck(False)
@cython.wraparound(False)
cdef long get_color(
        const long rank,
        const long file,
        const long [:, ::1] board_map):

    cdef long piece_val = board_map[rank, file]
    return 0 if piece_val == 0 else <long>copysign(1, piece_val)


@cython.boundscheck(False)
@cython.wraparound(False)
cpdef run_get_color(
        const long rank,
        const long file,
        const long [:, ::1] board_map):
    return get_color(rank=rank, file=file, board_map=board_map)


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
        long* general_position):

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


cpdef run_get_general_position(const long color, const long [:, ::1] board_map):

    cdef long[2] general_position
    cdef long general_found = get_general_position(
        color=color, board_map=board_map, general_position = general_position
    )

    if general_found:
        return general_position[0], general_position[1]


cdef is_in_castle(
        const long rank,
        const long file,
        const long* castle):

    return (castle[0] < rank < castle[1]) and (castle[2] < file < castle[3])


@cython.boundscheck(False)
@cython.wraparound(False)
cdef long get_all_spaces_occupied_by(
        const long color,
        const long [:, ::1] board_map,
        long* occupied_space_coords):

    cdef long num_occupied_spaces = 0
    cdef Py_ssize_t num_ranks = board_map.shape[0]
    cdef Py_ssize_t num_files = board_map.shape[1]
    cdef Py_ssize_t rank
    cdef Py_ssize_t file

    cdef long test_result

    for rank in range(num_ranks):
        for file in range(num_files):
            piece_color = (0 < board_map[rank, file]) - (board_map[rank, file] < 0)
            if piece_color == color:
                occupied_space_coords[2 * num_occupied_spaces] = rank
                occupied_space_coords[2 * num_occupied_spaces + 1] = file
                num_occupied_spaces += 1

    return num_occupied_spaces

@cython.boundscheck(False)
@cython.wraparound(False)
cpdef run_get_all_spaces_occupied_by(
        const long [:, ::1] board_map,
        const long color):

    cdef long[32] occupied_space_coords

    cdef long num_occupied_spaces = get_all_spaces_occupied_by(
        color=color,
        board_map=board_map,
        occupied_space_coords=occupied_space_coords)

    return [(occupied_space_coords[item], occupied_space_coords[item + 1])
            for item in range (0, (2 * num_occupied_spaces)) if (item %2 == 0)]


@cython.boundscheck(False)
@cython.wraparound(False)
cdef void search_spaces(
        const long [:, ::1] board_map,
        const long* start,
        const long* search_direction,
        long* num_empty_spaces,
        long* empty_spaces,
        long* has_first_occ_space,
        long* first_occupied):
    """

    :param board_map: (input) Cython memory view of game board numpy array
    :param start: (input) pointer to array with starting space coords
    :param search_direction: (input) pointer to array specifying move direction
    :param num_empty_spaces: (output) pointer (single element) array with
    number of empty spaces found in search direction.
    :param empty_spaces: (output) pointer to 1-D array containing coords of
    empty spaces. Each consecutive even/odd index pair --> one space
    :param has_first_occ_space: (output) pointer to integer/bool indicating whether
    or not any occupied spaces found in search direction.
    :param first_occupied: (output) pointer to array containing coords of first
    occupied space found in search direction (will be [-1, -1] if none found.
    :return: None
    """

    cdef long num_empty = 0
    cdef (long, long) next_step = (
        start[0] + search_direction[0], start[1] + search_direction[1])

    while (0 <= next_step[0] < board_map.shape[0]) and (
            0 <= next_step[1] < board_map.shape[1]) and (
            board_map[next_step[0], next_step[1]] == PColor.NUL):
        empty_spaces[2 * num_empty] = next_step[0]
        empty_spaces[2 * num_empty + 1] = next_step[1]
        num_empty += 1
        next_step = (
            next_step[0] + search_direction[0],
            next_step[1] + search_direction[1])
    if (0 <= next_step[0] < board_map.shape[0]) and (
            0 <= next_step[1] < board_map.shape[1]):
        has_first_occ_space[0] = 1
        first_occupied[0] = next_step[0]
        first_occupied[1] = next_step[1]
    num_empty_spaces[0] = num_empty


@cython.boundscheck(False)
@cython.wraparound(False)
cpdef run_search_space(
        const long [:, ::1] board_map,
        const long start_rank,
        const long start_file,
        const long dir_rank,
        const long dir_file):

    # cdef long [:, :] board_map_view = board_map

    cdef long search_start[2]
    search_start[0] = start_rank
    search_start[1] = start_file

    cdef long search_dir[2]
    search_dir[0] = dir_rank
    search_dir[1] = dir_file

    cdef long[1] num_empty_spaces = [0]

    cdef long empty_spaces[22]
    cdef int idx
    for idx in range(22):
        empty_spaces[idx] = -1


    cdef long[1] has_first_occ_space = [0]

    cdef long[2] first_occupied = [-1, -1]

    search_spaces(
        board_map=board_map,
        start=search_start,
        search_direction=search_dir,
        num_empty_spaces=num_empty_spaces,
        empty_spaces=empty_spaces,
        has_first_occ_space=has_first_occ_space,
        first_occupied=first_occupied
    )

    empty_space_list = [(empty_spaces[item], empty_spaces[item + 1]) for item
                        in range(0, (2 * num_empty_spaces[0])) if (item % 2 == 0)]

    if has_first_occ_space[0]:
        first_occ_tuple = (first_occupied[0], first_occupied[1])
    else:
        first_occ_tuple = None

    return {
        "empty_spaces": empty_space_list,
        "first_occupied_space": first_occ_tuple
        }

