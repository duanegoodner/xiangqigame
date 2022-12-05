import colorama
from xiangqigame.minimax_evaluators import PiecePoints
import xiangqigame.move_selectors as ms
from xiangqigame.game import Game
from xiangqigame.handlers.signals import set_signal_handlers
import xiangqigame.piece_points as pts
from xiangqigame.players import HumanPlayer, AIPlayer

from cpp_modules.game_board_py import PieceColor


def run():
    set_signal_handlers()
    colorama.init()

    # black_move_selector = ms.RandomMoveSelector()

    base_pts = pts.BasePoints(piece_vals=pts.base_pts_icga_2004)
    position_pts = pts.PositionPts(
        pts_arrays_black=pts.position_points_icga_2004)
    pts_evaluator = PiecePoints(base_pts=base_pts, position_pts=position_pts,
                                position_multiplier=1)

    red_move_selector = ms.MinimaxMoveSelector(
        evaluator=pts_evaluator, search_depth=2)
    black_move_selector = ms.MinimaxMoveSelector(
        evaluator=pts_evaluator, search_depth=3)

    my_game = Game(
        # red_player=HumanPlayer(color=PColor.RED),
        # black_player=HumanPlayer(color=PColor.BLK)
        red_player=AIPlayer(
            color=PieceColor.kRed, move_selector=red_move_selector),
        black_player=AIPlayer(
            color=PieceColor.kBlk, move_selector=black_move_selector)
    )

    my_game.play()


if __name__ == '__main__':
    run()
