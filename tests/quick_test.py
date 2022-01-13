import json
from game.game import Game
from data import alg_games as ag
from user_io.single_move import convert_alg_move_list
from board.game_board import GameBoard

with open('../data/game_start.json') as game_info:
    game_config = json.load(game_info)

moves_a = convert_alg_move_list(ag.game_a)
game_a = Game(game_config, auto_moves=moves_a)
# print(game_a._board)
game_a.play_auto_moves()






