

cdef long space_idx_1d((long, long) space)


cdef struct CastleSpace:
    long rank
    long file


cdef struct GameBoardC:
    long num_ranks
    long num_files
    long* board_map
    long red_river_edge
    long black_river_edge
    CastleSpace** red_castle_spaces
    CastleSpace** black_castle_spaces


cdef CastleSpace* new_castle_space(long rank, long file)
cdef void destruct_castle_space(CastleSpace* self)


cdef long* new_board_map()
cdef void destruct_board_map(long* self)
cdef CastleSpace** new_castle_spaces(
        long min_rank, long max_rank,long min_file, long max_file)
cdef void destruct_castle_spaces(CastleSpace** self)
cdef GameBoardC* new_game_board_c()
cdef void destruct_game_board_c(GameBoardC* self)
