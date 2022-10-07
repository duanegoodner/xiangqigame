import numpy as np


DTYPE = np.intc


cpdef enum PType:
    RED = -1
    NUL = 0
    BLK = 1



cdef (int, int) search_spaces(
        int [:, :] board_map,
        int [:] start,
        int [:] search_direction,
        int [:, :] empty_spaces,
        int[:] first_occupied_space):

    cdef int num_empty = 0
    cdef has_first_occ = False
    cdef (int, int) next_step = (
        start[0] + search_direction[0], start[1] + search_direction[1])

    while (next_step[0] < board_map.shape[0]) and (
            next_step[1] < board_map.shape[1]) and (
            board_map[next_step[0], next_step[1]] == PType.NUL):
        empty_spaces[num_empty, :] = next_step[0]
        # empty_spaces[num_empty, 1] = next_step[1]
        num_empty += 1
        next_step = (
            next_step[0] + search_direction[0],
            next_step[1] + search_direction[1])
    if (next_step[0] < board_map.shape[0]) and (
            next_step[1] < board_map.shape[1]):
        has_first_occ = True
        first_occupied_space = next_step
        # first_occupied_space[1] = next_step[1]

    return num_empty, has_first_occ


def run_search_space(
        int [:, :] board_map,
        int [:] start,
        int [:] direction):

    cdef int [:, :] board_map_view = board_map

    cdef (int, int) num_empty_has_first_occ

    empty_spaces = np.zeros(20, dtype=DTYPE).reshape((10,2))
    cdef int [:, :] empty_spaces_view = empty_spaces
    first_occupied = np.zeros(2, dtype=DTYPE)
    cdef int [:] first_occupied_view = first_occupied

    num_empty_has_first_occ = search_spaces(
        board_map=board_map_view,
        start=start,
        search_direction=direction,
        empty_spaces=empty_spaces_view,
        first_occupied_space=first_occupied_view
    )

    return num_empty_has_first_occ








