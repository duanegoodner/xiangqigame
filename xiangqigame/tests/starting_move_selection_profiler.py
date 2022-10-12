import cProfile
import time
from typing import Dict

from xiangqigame.piece_definitions import PColor

from xiangqigame.game_board_new import GameBoard
from xiangqigame.move_selectors import PiecePoints, MinimaxMoveSelector
import xiangqigame.piece_points as pts


def select_red_starting_move(
        search_depth: int,
        base_pts_map: Dict = pts.base_pts_icga_2004,
        position_pts_map: Dict = pts.position_points_icga_2004):
    game_board = GameBoard()
    base_pts = pts.BasePoints(piece_vals=base_pts_map)
    position_pts = pts.PositionPts(
        pts_arrays_black=position_pts_map,
        pts_arrays_red=position_pts_map)
    evaluator = PiecePoints(
        base_pts=base_pts, position_pts=position_pts, position_multiplier=1)

    move_selector = MinimaxMoveSelector(
        evaluator=evaluator, search_depth=search_depth)
    start = time.time()
    proposed_move = move_selector.select_move(
        game_board=game_board, cur_player=PColor.RED, cur_moves=[]
    )
    end = time.time()
    print(f"Selected ** move {proposed_move} in {end - start} seconds")


if __name__ == "__main__":
    depth = 3
    select_red_starting_move(depth)
    # cProfile.run(
    #     f"select_red_starting_move({depth})",
    #     filename=f"red_starting_move_depth_{depth}")
