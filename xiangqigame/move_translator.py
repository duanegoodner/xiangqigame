import re
from typing import List, Dict, Tuple


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

    return rank, file


def boardspace_to_algebraic_space(board_space: Tuple[int, int]):
    alg_column = chr(board_space[1] + ord('a'))
    alg_row = str(board_space[0] + 1)
    return f"{alg_column}{alg_row}"


def convert_parsed_input_to_move(parsed_input: List[str]):
    board_space_from_to = [
        algebraic_space_to_boardspace(entry) for entry in parsed_input]
    return {"start": board_space_from_to[0], "end": board_space_from_to[1]}


def convert_move_to_input_str(move: Dict):
    from_str = boardspace_to_algebraic_space(move["start"])
    to_str = boardspace_to_algebraic_space(move["end"])
    return f"{from_str}, {to_str}"
