from typing import Dict

import numpy as np
from xiangqigame.enums import PieceType, PieceColor

base_pts_icga_2004 = {
    PieceType.GENERAL: 6000,
    PieceType.ADVISOR: 120,
    PieceType.ELEPHANT: 120,
    PieceType.CHARIOT: 600,
    PieceType.HORSE: 270,
    PieceType.CANNON: 285,
    PieceType.SOLDIER: 30
}


class BasePoints:

    def __init__(self, piece_vals: Dict[PieceType, int]):
        assert set(piece_vals.keys()) == set(PieceType.__members__.values()) - {PieceType.NULL_PIECE}
        self._piece_vals = piece_vals

    @property
    def vals(self):
        return self._piece_vals


class PositionPts:

    def __init__(
            self,
            pts_arrays_red: Dict[PieceType, np.array],
            pts_arrays_black: Dict[PieceType, np.array] = None
    ):
        assert set(pts_arrays_red.keys()) == set(
            PieceType.__members__.values()) - {PieceType.NULL_PIECE}
        if pts_arrays_black:
            assert set(pts_arrays_red.keys()) == set(
                PieceType.__members__.values()).remove(PieceType.NULL_PIECE)
        if pts_arrays_black is None:
            pts_arrays_black = {piece_type: np.flip(pts_array, axis=0) for
                                piece_type, pts_array in pts_arrays_red.items()}
        self._pts_arrays = {
            PieceColor.RED: pts_arrays_red,
            PieceColor.BLACK: pts_arrays_black}

    @property
    def vals(self):
        return self._pts_arrays


general_position_icga_2004 = np.zeros(90).reshape((10, 9))
advisor_position_icga_2004 = np.zeros(90).reshape((10, 9))
elephant_position_icga_2004 = np.zeros(90).reshape((10, 9))

chariot_position_icga_2004 = np.array([
    [-2, 10, 6, 14, 12, 14, 6, 10, -2],
    [8, 4, 8, 16, 8, 16, 8, 4, 8],
    [4, 8, 6, 14, 12, 14, 6, 8, 4],
    [6, 10, 8, 14, 14, 14, 8, 10, 6],
    [12, 16, 14, 20, 20, 20, 14, 16, 12],
    [12, 14, 12, 18, 18, 18, 12, 14, 12],
    [12, 18, 16, 22, 22, 22, 16, 18, 12],
    [12, 12, 12, 18, 18, 18, 12, 12, 12],
    [16, 20, 18, 24, 26, 24, 18, 20, 16],
    [14, 14, 12, 18, 16, 18, 12, 14, 14]
])

horse_position_icga_2004 = np.array([
    [0, -4, 0, 0, 0, 0, 0, -4, 0],
    [0, 2, 4, 4, -2, 4, 4, 2, 0],
    [4, 2, 8, 8, 4, 8, 8, 2, 4],
    [2, 6, 8, 6, 10, 6, 8, 6, 2],
    [4, 12, 16, 14, 12, 14, 16, 12, 4],
    [6, 16, 14, 18, 16, 18, 14, 16, 6],
    [8, 24, 18, 24, 20, 24, 18, 24, 8],
    [12, 14, 16, 20, 18, 20, 16, 14, 12],
    [4, 10, 28, 16, 8, 16, 28, 10, 4],
    [4, 8, 16, 12, 4, 12, 16, 8, 4]
])

cannon_position_icga_2004 = np.array([
    [0, 0, 2, 6, 6, 6, 2, 0, 0],
    [0, 2, 4, 6, 6, 6, 4, 2, 0],
    [4, 0, 8, 6, 10, 6, 8, 0, 4],
    [0, 0, 0, 2, 4, 2, 0, 0, 0],
    [-2, 0, 4, 2, 6, 2, 4, 0, -2],
    [0, 0, 0, 2, 8, 2, 0, 0, 0],
    [0, 0, -2, 4, 10, 4, -2, 0, 0],
    [2, 2, 0, -10, -8, -10, 0, 2, 2],
    [2, 2, 0, -4, -14, -4, 0, 2, 2],
    [6, 4, 0, -10, -12, -10, 0, 4, 6]
])

soldier_position_icga_2004 = np.array([
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, -2, 0, 0, 0, -2, 0, 0],
    [2, 0, 8, 0, 8, 0, 8, 0, 2],
    [6, 12, 18, 18, 20, 18, 18, 12, 6],
    [10, 20, 30, 34, 40, 34, 30, 20, 10],
    [14, 26, 42, 60, 80, 60, 42, 26, 14],
    [18, 36, 56, 80, 120, 80, 56, 36, 18],
    [0, 3, 6, 9, 12, 9, 6, 3, 0]
])

position_points_icga_2004 = {
    PieceType.GENERAL: general_position_icga_2004,
    PieceType.ADVISOR: advisor_position_icga_2004,
    PieceType.ELEPHANT: elephant_position_icga_2004,
    PieceType.CHARIOT: chariot_position_icga_2004,
    PieceType.HORSE: horse_position_icga_2004,
    PieceType.CANNON: cannon_position_icga_2004,
    PieceType.SOLDIER: soldier_position_icga_2004
}
