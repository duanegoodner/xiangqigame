import re
import pprint
from typing import List


def transform_moves(moves: List[str]) -> List[str]:
    """
    Transforms a list of Xiangqi algebraic notation moves by correcting the
    rank ordering. The ranks in the input are given with rank 1 on the black
    side, which is incorrect. This function adjusts the ranks so that rank 1
    is on the red side, which is the conventional ordering in Xiangqi.

    :param moves: A list of moves in algebraic notation, where each move is
                  a string with file (letter) and rank (number) pairs.
    :type moves: list of str
    :return: A new list with ranks corrected to the proper ordering.
    """
    transformed_moves = []

    # Regular expression to match letters followed by numbers
    pattern = re.compile(r"([a-zA-Z]+)(\d+)")

    for move in moves:
        # Split the move into its two components
        parts = move.split(", ")
        transformed_parts = []

        for part in parts:
            # Substitute each part's numeric value
            new_part = pattern.sub(
                lambda m: f"{m.group(1)}{11 - int(m.group(2))}", part
            )
            transformed_parts.append(new_part)

        # Join the transformed parts back
        transformed_move = ", ".join(transformed_parts)
        transformed_moves.append(transformed_move)

    return transformed_moves


if __name__ == "__main__":
    original_move_list = [
        "a7, a6",
        "a4, a5",
        "a6, a5",
        "b3, b5",
        "a5, b5",
        "c4, c5",
        "b5, c5",
        "c1, a3",
        "e7, e6",
        "e4, e5",
        "e6, e5",
        "h1, g3",
        "g10, e8",
        "g3, e4",
        "e5, e4",
        "g1, i3",
        "a10, a3",
        "e1, e2",
        "a3, e3",
        "e2, f2",
        "i10, i9",
        "f1, e2",
        "e3, e2",
        "f2, f3",
        "i9, f9",
    ]

    updated_move_list = transform_moves(original_move_list)

    pprint.pprint(updated_move_list)
