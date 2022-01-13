import json
from game.game import Game
from board.move import Move
from board.board_space import BoardSpace
from data import alg_games as ag
from user_io.single_move import convert_alg_move_list

with open('../data/game_start.json') as game_info:
    game_config = json.load(game_info)

moves_a = convert_alg_move_list(ag.game_a)
game_a = Game(game_config, auto_moves=moves_a)
game_a.play_auto_moves()




