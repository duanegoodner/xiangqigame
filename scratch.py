from enum import Enum, auto
from board_space import BoardSpace
import numpy as np


board_space = BoardSpace(1, 1)
unit_delta = BoardSpace(0, 1)

sum = board_space + unit_delta

print(BoardSpace(board_space.rank + unit_delta.rank,
                 board_space.file + unit_delta.file))

