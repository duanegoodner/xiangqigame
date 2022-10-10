import cython


# mimic NamedTuple definition as cdef enum so search_spaces can be pure C
cpdef enum PType:
    RED = -1
    NUL = 0
    BLK = 1


@cython.boundscheck(False)
@cython.wraparound(False)
cdef void search_spaces(
        long [:, :] board_map,
        long* start,
        long* search_direction,
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
            board_map[next_step[0], next_step[1]] == PType.NUL):
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
def run_search_space(
        long [:, :] board_map,
        long start_rank,
        long start_file,
        long dir_rank,
        long dir_file):

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
