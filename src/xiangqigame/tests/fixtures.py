from typing import Dict

import pytest
from xiangqigame.game_board_new import GameBoard
from xiangqigame.move_selectors import PiecePoints, MinimaxMoveSelector
import xiangqigame.piece_points as pts


@pytest.fixture
def starting_game_board() -> GameBoard:
    return GameBoard()


@pytest.fixture
def minimax_move_selector(
        search_depth: int,
        base_pts_map: Dict = pts.base_pts_icga_2004,
        position_pts_map: Dict = pts.position_points_icga_2004):
    base_pts = pts.BasePoints(piece_vals=base_pts_map)
    position_pts = pts.PositionPts(
        pts_arrays_black=position_pts_map,
        pts_arrays_red=position_pts_map)
    evaluator = PiecePoints(
        base_pts=base_pts, position_pts=position_pts, position_multiplier=1)
    return MinimaxMoveSelector(evaluator=evaluator, search_depth=search_depth)

