import numpy as np
from xiangqigame.board_rules_new import PType, PColor


class StartingBoardBuilder:
    _back_row = (
        PType.CHA, PType.HOR, PType.ELE, PType.ADV, PType.GEN,
        PType.ADV, PType.ELE, PType.HOR, PType.CHA)
    _no_pieces_row = (
        PType.NUL, PType.NUL, PType.NUL, PType.NUL, PType.NUL,
        PType.NUL, PType.NUL, PType.NUL, PType.NUL)
    _cannon_row = (
        PType.NUL, PType.CAN, PType.NUL, PType.NUL, PType.NUL,
        PType.NUL, PType.NUL, PType.CAN, PType.NUL)
    _soldier_row = (
        PType.SOL, PType.NUL, PType.SOL, PType.NUL, PType.SOL,
        PType.NUL, PType.SOL, PType.NUL, PType.SOL)

    @property
    def starting_block(self):
        return np.vstack((
            self._back_row,
            self._no_pieces_row,
            self._cannon_row,
            self._soldier_row,
            self._no_pieces_row))

    def build_initial_board(self):
        black_half = PColor.BLK * self.starting_block
        red_half = PColor.RED * np.flip(self.starting_block, axis=0)
        return np.vstack((black_half, red_half))