import numpy as np
import cython


DTYPE = np.intc


cpdef enum PType:
    RED = -1
    NUL = 0
    BLK = 1


@cython.boundscheck(False)
@cython.wraparound(False)
cdef void search_spaces(
        int [:, :] board_map,
        int* start,
        int* search_direction,
        int* num_empty_spaces,
        int* empty_spaces,
        int* has_first_occ_space,
        int* first_occupied):

    cdef int num_empty = 0
    cdef has_first_occ = False
    cdef (int, int) next_step = (
        start[0] + search_direction[0], start[1] + search_direction[1])

    while (next_step[0] < board_map.shape[0]) and (
            next_step[1] < board_map.shape[1]) and (
            board_map[next_step[0], next_step[1]] == PType.NUL):
        empty_spaces[2 * num_empty] = next_step[0]
        empty_spaces[2 * num_empty + 1] = next_step[1]
        num_empty += 1
        next_step = (
            next_step[0] + search_direction[0],
            next_step[1] + search_direction[1])
    if (next_step[0] < board_map.shape[0]) and (
            next_step[1] < board_map.shape[1]):
        has_first_occ = 1
        first_occupied[0] = next_step[0]
        first_occupied[1] = next_step[1]
    num_empty_spaces[0] = num_empty


@cython.boundscheck(False)
@cython.wraparound(False)
def run_search_space(
        int [:, :] board_map,
        int start_rank,
        int start_file,
        int dir_rank,
        int dir_file):

    cdef int [:, :] board_map_view = board_map

    cdef int search_start[2]
    search_start[0] = start_rank
    search_start[1] = start_file

    cdef int search_dir[2]
    search_dir[0] = dir_rank
    search_dir[1] = dir_file

    cdef int[1] num_empty_spaces

    cdef int empty_spaces[22]

    cdef int[1] has_first_occ_space = [0]

    cdef int first_occupied[2]

    search_spaces(
        board_map=board_map_view,
        start=search_start,
        search_direction=search_dir,
        num_empty_spaces=num_empty_spaces,
        empty_spaces=empty_spaces,
        has_first_occ_space=has_first_occ_space,
        first_occupied=first_occupied
    )

    empty_space_list = [(empty_spaces[item], empty_spaces[item + 1]) for item
                        in range(0, num_empty_spaces[0] + 2, 2)]

    if has_first_occ_space[0]:
        first_occ_tuple = (first_occupied[0], first_occupied[1])
    else:
        first_occ_tuple = None

    return {
        "empty_spaces": empty_space_list,
        "first_occupied": first_occ_tuple
        }
