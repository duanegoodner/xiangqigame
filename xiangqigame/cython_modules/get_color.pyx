import cython
from libc.math cimport copysign

# mimic NamedTuple definition as cdef enum so search_spaces can be pure C
cdef enum PType:
    RED = -1
    NUL = 0
    BLK = 1


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
def run_get_color(
        const long rank,
        const long file,
        const long [:, ::1] board_map):
    return get_color(rank=rank, file=file, board_map=board_map)