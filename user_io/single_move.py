from typing import List
import user_io.notation_converter as nc
from board.board_space import BoardSpace
from board.move import Move
import user_io.messages as msg


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


def get_proposed_move():
    valid_input = None

    while not valid_input:
        user_input = input(msg.input_prompt)
        parsed_input = user_input.strip().replace(',', ' ').split()

        if valid_input_syntax(parsed_input):
            valid_input = parsed_input
        else:
            print('Invalid notation')

    raw_spaces = [nc.alg_to_indices(alg_entry) for alg_entry in valid_input]
    return Move(start=BoardSpace(*raw_spaces[0]),
                end=BoardSpace(*raw_spaces[1]))



