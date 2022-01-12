import json
from game.game import Game
from board.move import Move
from board.board_space import BoardSpace

with open('../data/game_start.json') as game_info:
    game_config = json.load(game_info)
my_game = Game(game_config)

test_moves = [
    Move(start=(BoardSpace(rank=3, file=0)), end=BoardSpace(rank=4, file=0)),
    Move(start=(BoardSpace(rank=6, file=0)), end=BoardSpace(rank=5, file=0)),
    Move(start=(BoardSpace(rank=4, file=0)), end=BoardSpace(rank=5, file=0))


]

print(my_game._board)
my_game._board.execute_move(test_moves[0])
print(my_game._board)
my_game._board.execute_move(test_moves[1])
print(my_game._board)
my_game._board.execute_move(test_moves[2])
print(my_game._board)
