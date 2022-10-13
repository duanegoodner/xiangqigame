from libc.stdlib cimport malloc, free
from .board_layout cimport BoardDim, RiverEdges, CastleEdges
from .piece_definitions cimport PC, PT


cdef struct Castle:
    long min_rank
    long max_rank
    long min_file
    long max_file


cdef struct CastleSpace:
    long rank
    long file


cdef CastleSpace* new_castle_space(long rank, long file):
    cdef CastleSpace* self = <CastleSpace*> malloc(sizeof(CastleSpace))
    self.rank = rank
    self.file = file
    return self


cdef void destruct_castle_space(CastleSpace* self):
    free(<void*> self)


cdef struct GameBoardC:
    long num_ranks
    long num_files
    long* board_map
    long red_river_edge
    long black_river_edge
    CastleSpace** red_castle_spaces
    CastleSpace** black_castle_spaces


cdef long* new_board_map():
    cdef long[10 * 9] starting_pieces = [
        PT.CHA, PT.HOR, PT.ELE, PT.ADV, PT.GEN, PT.ADV, PT.ELE, PT.HOR, PT.CHA,
        PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
        PT.NNN, PT.CAN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.CAN, PT.NNN,
        PT.SOL, PT.NNN, PT.SOL, PT.NNN, PT.SOL, PT.NNN, PT.SOL, PT.NNN, PT.SOL,
        PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
        PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
        -PT.SOL, PT.NNN, -PT.SOL, PT.NNN, -PT.SOL, PT.NNN, -PT.SOL, PT.NNN,
        -PT.SOL,
        PT.NNN, -PT.CAN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, -PT.CAN,
        PT.NNN,
        PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
        -PT.CHA, -PT.HOR, -PT.ELE, -PT.ADV, PT.GEN, -PT.ADV, -PT.ELE, -PT.HOR,
        -PT.CHA
    ]

    cdef long* self = <long*> malloc(
        BoardDim.NUM_RANKS * BoardDim.NUM_FILES * sizeof(long))
    cdef long board_idx_1d
    for board_idx_1d in range(BoardDim.NUM_RANKS * BoardDim.NUM_FILES):
        self[board_idx_1d] = starting_pieces[board_idx_1d]

    return self


cdef void destruct_board_map(long* self):
    free(<void*> self)


cdef Castle* new_castle(long min_rank, long max_rank,
                         long min_file, long max_file):
    cdef Castle* self = <Castle*> malloc(sizeof(Castle))
    self.min_rank = min_rank
    self.max_rank = max_rank
    self.min_file = min_file
    self.max_file = max_file

    return self


cdef void destruct_castle(Castle* self):
    free(<void*> self)


cdef CastleSpace** new_castle_spaces(
        long min_rank, long max_rank,long min_file, long max_file):
    cdef CastleSpace** self = <CastleSpace**> malloc(9 * sizeof(CastleSpace*))
    cdef long rank
    cdef long file
    cdef CastleSpace* cur_space
    for rank in range(min_rank, max_rank + 1):
        for file in range(min_file, max_file + 1):
            cur_space = new_castle_space(rank, file)
            self[3 * rank + file] = cur_space
    return self


cdef void destruct_castle_spaces(CastleSpace** self):
    cdef long space
    for space in range(9):
        destruct_castle_space(self[space])
    free(<void*> self)


cdef GameBoardC* new_game_board_c():
    cdef GameBoardC* self = <GameBoardC*> malloc(sizeof(GameBoardC))
    self.num_ranks = BoardDim.NUM_RANKS
    self.num_files = BoardDim.NUM_FILES
    self.red_river_edge = RiverEdges.RED
    self.black_river_edge = RiverEdges.BLK
    self.board_map = new_board_map()
    self.red_castle_spaces = new_castle_spaces(
        min_rank=CastleEdges.RED_MIN_RANK,
        max_rank=CastleEdges.RED_MAX_RANK,
        min_file=CastleEdges.RED_MIN_FILE,
        max_file=CastleEdges.RED_MAX_FILE)
    self.black_castle_spaces = new_castle_spaces(
        min_rank=CastleEdges.BLK_MIN_RANK,
        max_rank=CastleEdges.BLK_MAX_RANK,
        min_file=CastleEdges.BLK_MIN_FILE,
        max_file=CastleEdges.BLK_MAX_FILE)

    return self


cdef void destruct_game_board_c(GameBoardC* self):
    destruct_castle_spaces(self.red_castle_spaces)
    destruct_castle_spaces(self.black_castle_spaces)
    destruct_board_map(self.board_map)
    free(<void*> self)


cpdef void create_and_destroy_game_board_c():
    cdef GameBoardC* my_game_board = new_game_board_c()
    print(my_game_board.board_map[0])
    destruct_game_board_c(my_game_board)