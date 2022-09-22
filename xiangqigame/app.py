import colorama
import json
import pkgutil
import xiangqigame.move_selectors as ms
from xiangqigame.enums import PieceColor
from xiangqigame.game import Game
from xiangqigame.handlers.signals import set_signal_handlers
from xiangqigame.players import HumanPlayer, AIPlayer
import xiangqigame.data


def run():
    set_signal_handlers()
    colorama.init()

    game_config = json.loads(pkgutil.get_data('xiangqigame.data', 'game_start.json'))

    my_game = Game(
        game_config=game_config,
        red_player=AIPlayer(
            color=PieceColor.RED, move_selector=ms.random_move),
        black_player=AIPlayer(
            color=PieceColor.BLACK, move_selector=ms.random_move))

    my_game.play()


if __name__ == '__main__':
    run()
