from .piece_definitions cimport PC, PT


cdef enum BoardDim:
    NUM_RANKS = 10
    NUM_FILES = 9


cdef enum RiverEdges:
    BLK = 4
    RED = 5


cdef enum CastleEdges:
    RED_MIN_RANK = 7
    RED_MAX_RANK = 9
    RED_MIN_FILE = 3
    RED_MAX_FILE = 5
    BLK_MIN_RANK = 0
    BLK_MAX_RANK = 2
    BLK_MIN_FILE = 3
    BLK_MAX_FILE = 5


# cdef long[10 * 9] starting_pieces = [
#         PT.CHA, PT.HOR, PT.ELE, PT.ADV,
#         PT.GEN,
#         PT.ADV, PT.ELE, PT.HOR, PT.CHA,
#
#         PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
#
#         PT.NNN, PT.CAN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
#         PT.CAN, PT.NNN,
#
#         PT.SOL, PT.NNN, PT.SOL, PT.NNN, PT.SOL,
#         PT.NNN, PT.SOL, PT.NNN, PT.SOL,
#
#         PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
#
#         PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
#
#         -PT.SOL, PT.NNN, -PT.SOL, PT.NNN, -PT.SOL,
#         PT.NNN, -PT.SOL, PT.NNN, -PT.SOL,
#
#         PT.NNN, -PT.CAN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
#         -PT.CAN, PT.NNN,
#
#         PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
#
#         -PT.CHA, -PT.HOR, -PT.ELE, -PT.ADV,
#         PT.GEN,
#         -PT.ADV, -PT.ELE, -PT.HOR, -PT.CHA
#     ]




# cdef long[10 * 9] starting_pieces = [
#         100, PT.HOR, PT.ELE, PT.ADV, PT.GEN, PT.ADV, PT.ELE, PT.HOR, PT.CHA,
#         PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
#         PT.NNN, PT.CAN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.CAN, PT.NNN,
#         PT.SOL, PT.NNN, PT.SOL, PT.NNN, PT.SOL, PT.NNN, PT.SOL, PT.NNN, PT.SOL,
#         PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
#         PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
#         -PT.SOL, PT.NNN, -PT.SOL, PT.NNN, -PT.SOL, PT.NNN, -PT.SOL, PT.NNN, -PT.SOL,
#         PT.NNN, -PT.CAN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, -PT.CAN, PT.NNN,
#         PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN, PT.NNN,
#         -PT.CHA, -PT.HOR, -PT.ELE, -PT.ADV, PT.GEN, -PT.ADV, -PT.ELE, -PT.HOR, -PT.CHA
#     ]



