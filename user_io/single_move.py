from typing import List
import common.notation_converter as nc
from board.board_space import BoardSpace
from board.move import Move
import user_io.messages as msg
from common.enums import Out


def is_valid_space(alg_entry):
    file_char = alg_entry[0]
    rank_chars = alg_entry[1:]

    if file_char in nc.file_index_of.keys() and rank_chars in \
            nc.rank_index_of.keys():
        return True
    else:
        return False


def valid_input_syntax(parsed_input: List[str]):
    return all([is_valid_space(alg_entry) for alg_entry in parsed_input])


def build_move(alg_move: str):
    parsed_alg_move = alg_move.strip().replace(',', ' ').split()
    raw_spaces = [nc.alg_to_indices(alg_entry) for alg_entry in
                  parsed_alg_move]
    return Move(start=BoardSpace(*raw_spaces[0]),
                end=BoardSpace(*raw_spaces[1]))


def convert_alg_move_list(alg_move_list: List[str]):
    return [build_move(alg_move) for alg_move in alg_move_list]


def get_proposed_move():
    valid_input = None

    while not valid_input:
        user_input = input(msg.Out.INPUT_PROMPT)
        parsed_input = user_input.strip().replace(',', ' ').split()

        if valid_input_syntax(parsed_input):
            valid_input = parsed_input
        else:
            msg.output(Out.ILLEGAL_MOVE)

    raw_spaces = [nc.alg_to_indices(alg_entry) for alg_entry in valid_input]
    return Move(start=BoardSpace(*raw_spaces[0]),
                end=BoardSpace(*raw_spaces[1]))
