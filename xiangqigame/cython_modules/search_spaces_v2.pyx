import numpy
import cython


DTYPE = numpy.intc


cpdef enum PType:
    RED = -1
    NUL = 0
    BLK = 1


cdef struct SpaceSearchResults:
    int num_empty_spaces
    int first_occ_space_rank
    int first_occ_space_file


@cython.boundscheck(False)
@cython.wraparound(False)
cdef void search_spaces(
        int [:, :] board_map,
        int start_rank,
        int start_file,
        int dir_rank,
        int dir_file,
        SpaceSearchResults* results,
        int [:, :] empty_spaces
):

    cdef (int, int) next_step = (
        start_rank + dir_rank, start_file + dir_file)

    cdef int num_empty = 0

    while (next_step[0] < board_map.shape[0]) and (
            next_step[1] < board_map.shape[1]) and (
            board_map[next_step[0], next_step[1]] == PType.NUL):
        empty_spaces[num_empty, 0] = next_step[0]
        empty_spaces[num_empty, 1] = next_step[1]
        num_empty += 1
        next_step = (
            next_step[0] + dir_rank,
            next_step[1] + dir_file)
    if (next_step[0] < board_map.shape[0]) and (
            next_step[1] < board_map.shape[1]):
        results.first_occ_space_rank = next_step[0]
        results.first_occ_space_file = next_step[1]


@cython.boundscheck(False)
@cython.wraparound(False)
def run_search_space(
        int [:, :] board_map,
        int start_rank,
        int start_file,
        int dir_rank,
        int dir_file):

    cdef int [:, :] board_map_view = board_map
    cdef SpaceSearchResults results

    empty_spaces = numpy.full((11, 2), -1, dtype=numpy.intc)
    cdef int [:, :] empty_spaces_view = empty_spaces

    results.num_empty_spaces = 0
    results.first_occ_space_rank = -1
    results.first_occ_space_file = -1

    num_empty_has_first_occ = search_spaces(
        board_map=board_map_view,
        start_rank=start_rank,
        start_file=start_file,
        dir_rank=dir_rank,
        dir_file=dir_file,
        results = &results,
        empty_spaces=empty_spaces_view
    )

