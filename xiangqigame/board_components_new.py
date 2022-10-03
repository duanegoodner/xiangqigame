from typing import NamedTuple, Tuple, List, Union
from xiangqigame.board_utilities_new import RULES


class BoardVector(NamedTuple):
    rank: int
    file: int


class BoardSpace(BoardVector):

    @classmethod
    def from_plain_tuple(cls, plain_tuple: Tuple):
        return cls(rank=plain_tuple[0], file=plain_tuple[1])

    def __add__(self, other):
        return __class__(
            rank=self.rank + other.rank, file=self.file + other.file)

    def is_in_castle_of(self, color: int):
        castle = RULES.castle_edges[color]
        return (castle.min_rank <= self.rank <= castle.max_rank) and (
            castle.min_file <= self.file <= castle.max_file)


class SpaceSearchResult(NamedTuple):
    empty_spaces: List[BoardSpace]
    first_occupied_space: Union[BoardSpace, None]
