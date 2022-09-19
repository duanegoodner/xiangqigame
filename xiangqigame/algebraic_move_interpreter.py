import re
from typing import List

from xiangqigame.move import Move

from xiangqigame.board_components import BoardSpace


def parse_input(player_input: str) -> List[str]:
    first_step = player_input.strip().split(',')
    return [item.strip() for item in first_step]


def is_valid_algebraic_pair(parsed_input: List[str]) -> bool:
    return len(parsed_input) == 2 and all([
        re.fullmatch('^[a-i](?:10|[1-9])$', item) for item in parsed_input
    ])


def algebraic_space_to_boardspace(algebraic_space: str):
    alg_column = algebraic_space[0]
    file = ord(alg_column) - ord('a')

    alg_row = algebraic_space[1:]
    rank = int(alg_row) - 1

    return BoardSpace(rank=rank, file=file)


def convert_parsed_input_to_move(parsed_input: List[str]):
    board_space_from_to = [
        algebraic_space_to_boardspace(entry) for entry in parsed_input]
    return Move(start=board_space_from_to[0], end=board_space_from_to[1])


