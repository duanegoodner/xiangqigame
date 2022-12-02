import cpp_modules.game_board_py.GameBoardPy as gbp


# def test_game_board_init():
#     my_game_board = gbp.GameBoard()


# test_game_board_init()


import time

my_game_board = gbp.GameBoard()
my_start_space = gbp.BoardSpace(5, 5)
my_end_space = gbp.BoardSpace(5, 8)
my_move = gbp.Move(my_start_space, my_end_space)

print(my_game_board)
print(my_move.start.rank, my_move.start.file)
print(my_move.end.rank, my_move.end.file)

is_red_in_check = my_game_board.IsInCheck(gbp.PieceColor.kRed)
print(is_red_in_check)

all_spaces_occ_by_red = my_game_board.GetAllSpacesOccupiedBy(gbp.PieceColor.kRed)
print(all_spaces_occ_by_red)

moves_of_red = my_game_board.CalcFinalMovesOf(gbp.PieceColor.kRed)
print(moves_of_red)
print(len(moves_of_red))

start_time = time.time()
for i in range(100):
    my_game_board.CalcFinalMovesOf(gbp.PieceColor.kRed)
end_time = time.time()
delta = end_time - start_time
print(delta)