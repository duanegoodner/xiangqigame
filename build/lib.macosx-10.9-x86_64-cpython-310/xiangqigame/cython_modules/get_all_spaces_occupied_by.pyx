import cython


# mimic NamedTuple definition as cdef enum so search_spaces can be pure C
cpdef enum PType:
    RED = -1
    NUL = 0
    BLK = 1

@cython.boundscheck(False)
@cython.wraparound(False)
cdef long get_all_spaces_occupied_by(
        long color,
        long [:, :] board_map,
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
def run_get_all_spaces_occupied_by(
        long [:, :] board_map,
        long color):

    cdef long[32] occupied_space_coords

    num_occupied_spaces = get_all_spaces_occupied_by(
        color=color,
        board_map=board_map,
        occupied_space_coords=occupied_space_coords)

    return [(occupied_space_coords[item], occupied_space_coords[item + 1])
            for item in range (0, (2 * num_occupied_spaces)) if (item %2 == 0)]










