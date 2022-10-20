from libc.math cimport copysign
from libc.stdlib cimport labs, malloc, free
from .board_layout cimport BoardDim, RiverEdges, CastleEdges
from .piece_definitions cimport PC, PT


cdef long space_idx_1d((long, long) space):
    cdef long idx_1d = BoardDim.NUM_FILES * space[0] + space[1]
    return idx_1d


cdef (long, long) space_idx_2d(long space_idx_1d):
    cdef long rank = space_idx_1d / BoardDim.NUM_FILES
    cdef long file = space_idx_1d % BoardDim.NUM_FILES
    return rank, file


cdef CastleSpace* new_castle_space(long rank, long file):
    cdef CastleSpace* self = <CastleSpace*> malloc(sizeof(CastleSpace))
    self.rank = rank
    self.file = file
    return self


cdef void destruct_castle_space(CastleSpace* self):
    free(<void*> self)


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
    print("red castle spaces destructed")
    destruct_castle_spaces(self.black_castle_spaces)
    print("black castle spaces destructed")
    destruct_board_map(self.board_map)
    print("board_map destructed")
    free(<void*> self)


cdef bint is_occupied((long, long) space, GameBoardC* self):
    return self.board_map[space_idx_1d(space)] != PT.NNN


cdef long get_type((long, long) space, GameBoardC* self):
    return labs(self.board_map[space_idx_1d(space)])


cdef long get_color((long, long) space, GameBoardC* self):
    cdef long piece_val = self.board_map[space_idx_1d(space)]
    return 0 if piece_val == 0 else <long>copysign(1, piece_val)


cpdef void create_and_destroy_game_board_c():
    cdef GameBoardC* my_game_board = new_game_board_c()
    print(my_game_board.board_map[0])
    cdef (long, long) space = (0, 1)
    print(space)
    cdef long color_0_1 = get_color(space=space, self=my_game_board)
    print(color_0_1)
    destruct_game_board_c(my_game_board)