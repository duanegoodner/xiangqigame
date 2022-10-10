import json
from pathlib import Path
import alg_games as ag



file_map = {
    "a": "i",
    "b": "h",
    "c": "g",
    "d": "f",
    "e": "e",
    "f": "d",
    "g": "c",
    "h": "b",
    "i": "a"
}


def parse_entry(old_input: str):
    first_step = old_input.split(",")
    return [item.strip() for item in first_step]


def update_rank(old_rank: str):
    return str(11 - int(old_rank))


def update_file(old_file: str):
    return old_file
    # return file_map[old_file]


def update_space(old_space: str):
    new_file = update_file(old_space[0])
    new_rank = update_rank(old_space[1:])
    return f"{new_file}{new_rank}"


def update_move_list(old_list: list[str]):
    parsed_entries = [parse_entry(item) for item in old_list]
    new_pairs = [[update_space(space) for space in pair] for pair in parsed_entries]
    new_move_inputs = [", ".join(pair) for pair in new_pairs]

    return new_move_inputs


updated_game_a = update_move_list(ag.game_a)
with Path("game_a.py").open(mode="w") as f:
    json.dump(updated_game_a, f)

