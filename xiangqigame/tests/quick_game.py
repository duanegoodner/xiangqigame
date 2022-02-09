import json
import pkgutil
from xiangqigame.game import Game
from xiangqigame.data import alg_games as ag
from xiangqigame.user_io.single_move import convert_alg_move_list

game_config = json.loads(
        pkgutil.get_data('xiangqigame.data', 'game_start.json'))

moves_d = convert_alg_move_list(ag.game_d)
game_d = Game(game_config, auto_moves=moves_d)
game_d.play_auto_moves()
