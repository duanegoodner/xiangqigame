import numpy as np
from typing import TypeAlias
import xiangqigame_core as core


class PointsTypeDeterminer:
    _dtype_map = {
        (True, 1): np.int8,
        (True, 2): np.int16,
        (True, 4): np.int32,
        (True, 8): np.int64,
        (False, 1): np.uint8,
        (False, 2): np.uint16,
        (False, 4): np.uint32,
        (False, 8): np.uint64,
    }

    def get_points_type(self) -> type:
        dtype_key = (core.is_signed_points_type(), core.size_of_points_type())
        return self._dtype_map[dtype_key]


