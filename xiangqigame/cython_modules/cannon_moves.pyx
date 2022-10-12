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

    cdef long search_start[2]
    search_start[0] = start_rank
    search_start[1] = start_file

    # cdef long[34] all_dest_coords
    # cdef long all_dest_count
    cdef long new_coord
    cdef long[2] cur_search_dir
    # cdef long[18] cur_search_a_coords
    # cdef long[1] cur_search_a_empty_space_count = [0]
    # cdef long[1] cur_search_a_has_first_occ_space = [0]
    # cdef long[2] cur_search_a_first_occ = [-1, -1]
    # cdef long[18] cur_search_b_coords
    # cdef long[1] cur_search_b_empty_space_count = [0]
    # cdef long[1] cur_search_b_has_first_occ_space = [0]
    # cdef long[2] cur_search_b_first_occ = [-1, -1]

    cdef long direction
    
    for direction in range(4):


        cdef long* a_empty_space_count = <long*> malloc(sizeof(long))
        cdef long* a_empty_space_coords = <long*> malloc(18 * sizeof(long))
        cdef long* a_has_first_occ_space = <long*> malloc(sizeof(long))
        cdef long* a_first_occ_coords = <long*> malloc(2 * sizeof(long))
        cdef long* b_empty_space_count = <long*> malloc(sizeof(long))
        cdef long* b_empty_space_coords = <long*> malloc(18 * sizeof(long))
        cdef long* b_has_first_occ_space = <long*> malloc(sizeof(long))
        cdef long * b_first_occ_coords = <long *> malloc(2 * sizeof(long))

        rank_idx = 2 * direction
        file_idx = 2 * direction + 1
        
        cur_search_dir[0] = orthog_directions[rank_idx]
        cur_search_dir[1] = orthog_directions[file_idx]
        
        search_spaces(
            board_map=board_map,
            start=search_start,
            search_direction=cur_search_dir,
            num_empty_spaces=a_empty_space_count,
            empty_spaces=a_empty_space_coords,
            has_first_occ_space=a_has_first_occ_space,
            first_occupied=a_first_occ_coords)

        # print(cur_search_a_coords)

        for new_coord in range(2 * a_empty_space_count[0]):
            all_dest_coords[all_dest_count + new_coord] = a_empty_space_coords[new_coord]
        
        all_dest_count += a_empty_space_count[0]
        
        if a_has_first_occ_space[0]:
            search_spaces(
                board_map=board_map,
                start=a_first_occ_coords,
                search_direction=cur_search_dir,
                num_empty_spaces=b_empty_space_count,
                empty_spaces=b_empty_space_coords,
                has_first_occ_space=b_has_first_occ_space,
                first_occupied=b_first_occ_coords)
            if b_has_first_occ_space[0] and get_color(
                rank=b_first_occ_coords[0],
                file=b_first_occ_coords[1],
                board_map=board_map) == -1 * color:
                all_dest_coords[all_dest_count] = b_first_occ_coords[0]
                all_dest_coords[all_dest_count + 1] = b_first_occ_coords[1]
                all_dest_count += 1

        free(<void*> a_empty_space_count)
        free(<void*> a_empty_space_coords)
        free(<void*> a_has_first_occ_space)
        free(<void*> a_first_occ_coords)
        free(<void *> b_empty_space_count)
        free(<void *> b_empty_space_coords)
        free(<void *> b_has_first_occ_space)
        free(<void *> b_first_occ_coords)


        
    print(all_dest_count)
    return all_dest_count


def run_cannon_moves(
      const long from_rank,
      const long from_file,
      const long [:, ::1] board_map,
      const long color):

    destinations = np.full((4, 9), -1, dtype=np.int64)

    cdef long[34] all_dest_coords
    cdef long num_dests = cannon_moves(
        start_rank=from_rank,
        start_file=from_file,
        color=color,
        board_map=board_map,
        all_dest_coords=all_dest_coords)

    moves = [{
        "start": (from_rank, from_file),
        "end": (all_dest_coords[item], all_dest_coords[item + 1])
    } for item in range (0, (2 * num_dests)) if (item % 2 == 0)]

    return moves

            
            
            
        
        





