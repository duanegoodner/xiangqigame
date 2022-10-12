import numpy as np
from libc.stdlib cimport malloc, free
from .cython_board_utilities cimport search_spaces, get_color


cdef cannon_moves(
        const long start_rank,
        const long start_file,
        const long color,
        const long [:, ::1] board_map,
        long [:, ::1] all_dest_coords):

    cdef long[8] orthog_directions = [
        0, 1,
        0, -1,
        1, 0,
        -1, 0
    ]


cpdef run_cannon_moves(
        const long [:, ::1] board_map,
        const long start_rank,
        const long start_file,
        const long color)