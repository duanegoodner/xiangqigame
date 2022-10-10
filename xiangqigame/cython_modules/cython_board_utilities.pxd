

cdef enum PColor:
    RED = -1
    NUL = 0
    BLK = 1


cdef enum PType:
    # NUL = 0
    GEN = 1
    ADV = 2
    ELE = 3
    HOR = 4
    CHA = 5
    CAN = 6
    SOL = 7


cdef long get_color(
        const long rank,
        const long file,
        const long [:, ::1] board_map)


cpdef run_get_color(
        const long rank,
        const long file,
        const long [:, ::1] board_map)


cdef void get_castle_edges(
        const long color,
        long *castle_edges)


cdef long get_general_position(
        const long color,
        const long [:, ::1] board_map,
        long* general_position)


cdef is_in_castle(
        const long rank,
        const long file,
        const long[4] castle)


cdef long get_all_spaces_occupied_by(
        const long color,
        const long [:, ::1] board_map,
        long* occupied_space_coords)


cpdef run_get_all_spaces_occupied_by(
        const long [:, ::1] board_map,
        const long color)


cdef void search_spaces(
        const long [:, ::1] board_map,
        const long* start,
        const long* search_direction,
        long* num_empty_spaces,
        long* empty_spaces,
        long* has_first_occ_space,
        long* first_occupied)


cpdef run_search_space(
        const long [:, ::1] board_map,
        const long start_rank,
        const long start_file,
        const long dir_rank,
        const long dir_file)