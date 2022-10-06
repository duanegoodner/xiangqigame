import numpy as np
import pprint
from xiangqigame.piece_definitions import PType, PColor


class StartingBoardBuilder:
    _back_row = [
        PType.CHA, PType.HOR, PType.ELE, PType.ADV, PType.GEN,
        PType.ADV, PType.ELE, PType.HOR, PType.CHA]
    _no_pieces_row = [
        PType.NUL, PType.NUL, PType.NUL, PType.NUL, PType.NUL,
        PType.NUL, PType.NUL, PType.NUL, PType.NUL]
    _cannon_row = [
        PType.NUL, PType.CAN, PType.NUL, PType.NUL, PType.NUL,
        PType.NUL, PType.NUL, PType.CAN, PType.NUL]
    _soldier_row = [
        PType.SOL, PType.NUL, PType.SOL, PType.NUL, PType.SOL,
        PType.NUL, PType.SOL, PType.NUL, PType.SOL]

    @property
    def starting_block(self):
        return [
            self._back_row,
            self._no_pieces_row,
            self._cannon_row,
            self._soldier_row,
            self._no_pieces_row]

    def build_initial_board_list(self):
        black_half = [
            [PColor.BLK * item for item in row] for row in self.starting_block
        ]
        red_half = [
            [PColor.RED * item for item in row] for row in self.starting_block[::-1]
        ]
        return black_half + red_half

    def build_initial_board_array(self):
        return np.array(self.build_initial_board_list())


if __name__ == "__main__":
    pp = pprint.PrettyPrinter(indent=4)
    bb = StartingBoardBuilder()
    board_list = bb.build_initial_board_list()
    pp.pprint(board_list)
    print()
    board_array = bb.build_initial_board_array()
    pp.pprint(board_array)


