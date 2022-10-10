import cython

# mimic NamedTuple definition as cdef enum so search_spaces can be pure C
cpdef enum PType:
    RED = -1
    NUL = 0
    BLK = 1


@cython.boundscheck(False)
@cython.wraparound(False)
cdef long get_color(
        long rank,
        long file,
        long [:, :] board_map):
    return (0 < board_map[rank, file]) - (board_map[rank, file] < 0)


@cython.boundscheck(False)
@cython.wraparound(False)
def run_get_color(
        long rank,
        long file,
        long [:, :] board_map):
    return get_color(rank=rank, file=file, board_map=board_map)