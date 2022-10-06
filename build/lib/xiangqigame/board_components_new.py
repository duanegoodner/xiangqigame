from typing import NamedTuple, Tuple, List, Union


class UnComputableBoardSpace(NamedTuple):
    rank: int
    file: int


class BoardVector(NamedTuple):
    rank: int
    file: int


class BoardSpace(UnComputableBoardSpace):

    @classmethod
    def from_plain_tuple(cls, plain_tuple: Tuple):
        return cls(rank=plain_tuple[0], file=plain_tuple[1])

    def add_board_vector(self, board_vector: BoardVector):
        return __class__(self.rank + board_vector.rank,
                         self.file + board_vector.file)

    def __add__(self, other):
        return __class__(
            rank=self.rank + other.rank, file=self.file + other.file)




class SpaceSearchResult(NamedTuple):
    empty_spaces: List[BoardSpace]
    first_occupied_space: Union[BoardSpace, None]
