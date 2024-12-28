from pathlib import Path
from dataclasses import dataclass

full_game_moves = [
    "a4, a5",
    "a7, a6",
    "a5, a6",
    "b8, b6",
    "a6, b6",
    "c7, c6",
    "b6, c6",
    "c10, a8",
    "e4, e5",
    "e7, e6",
    "e5, e6",
    "h10, g8",
    "g1, e3",
    "g8, e7",
    "e6, e7",
    "g10, i8",
    "a1, a8",
    "e10, e9",
    "a8, e8",
    "e9, f9",
    "i1, i2",
    "f10, e9",
    "e8, e9",
    "f9, f8",
    "i2, f2",
]


@dataclass
class RedAndBlackMoves:
    red_moves: list[str]
    black_moves: list[str]


def split_moves(all_moves: list[str]) -> RedAndBlackMoves:
    return RedAndBlackMoves(
        red_moves=all_moves[::2],
        black_moves=all_moves[1::2],
    )


def write_moves_to_file(moves: list[str], path: Path) -> None:
    with path.open(mode="w") as output_file:
        for move in moves:
            output_file.write(f"{move}\n")


def split_moves_to_files(
    moves: list[str], red_output: Path, black_output: Path
):
    red_and_black_moves = split_moves(moves)
    write_moves_to_file(red_and_black_moves.red_moves, path=red_output)
    write_moves_to_file(red_and_black_moves.black_moves, path=black_output)


if __name__ == "__main__":
    split_moves_to_files(
        full_game_moves,
        red_output=Path("scripted_game_red_moves.txt"),
        black_output=Path("scripted_game_black_moves.txt"),
    )
