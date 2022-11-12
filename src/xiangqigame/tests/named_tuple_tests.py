from typing import NamedTuple, Union


class BoardVector(NamedTuple):
    rank: int
    file: int


class BoardSpace(BoardVector):
    def __add__(self, other):
        return __class__(rank=self.rank + other.rank, file=self.file + other.file)


space = BoardSpace(rank=0, file=0)
step = BoardVector(rank=3, file=0)

print(space + step)


class Result(NamedTuple):
    item_a: int
    item_b: Union[int, None]


res = Result(item_a=2, item_b=None)
print(res)


