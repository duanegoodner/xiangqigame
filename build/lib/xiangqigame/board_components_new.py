from typing import NamedTuple, Tuple, List, Union


class SpaceSearchResult(NamedTuple):
    empty_spaces: List[Tuple[int, int]]
    first_occupied_space: Union[Tuple[int, int], None]
