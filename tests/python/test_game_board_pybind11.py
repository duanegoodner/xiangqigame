import time

from xiangqigame_core import BoardSpace, GameBoard, Move, PieceColor

my_game_board = GameBoard()
my_start_space = BoardSpace(5, 5)
my_end_space = BoardSpace(5, 8)
my_move = Move(my_start_space, my_end_space)

print(my_game_board)
print(my_move.start.rank, my_move.start.file)
print(my_move.end.rank, my_move.end.file)

is_red_in_check = my_game_board.IsInCheck(PieceColor.kRed)
print(is_red_in_check)

all_spaces_occ_by_red = my_game_board.GetAllSpacesOccupiedBy(PieceColor.kRed)
print(all_spaces_occ_by_red)

moves_of_red = my_game_board.CalcFinalMovesOf(PieceColor.kRed)
print(moves_of_red)
print(moves_of_red.size())

start_time = time.time()
for i in range(100):
    my_game_board.CalcFinalMovesOf(PieceColor.kRed)
end_time = time.time()
delta = end_time - start_time
print(delta)