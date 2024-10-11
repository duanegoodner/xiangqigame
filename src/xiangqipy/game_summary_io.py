"""
@file game_summary_io.py

Contains functions for importing / exporting a GameSummary from / to .json file.
"""

import msgspec
import numpy as np
from pathlib import Path
from typing import Any, Type
from xiangqi_bindings import PieceColor, PieceType
from xiangqipy.game import GameSummary


def enc_hook(obj: Any) -> Any:
    if isinstance(obj, PieceColor):
        return int(obj)
    if isinstance(obj, PieceType):
        return int(obj)
    if isinstance(obj, np.ndarray):
        return obj.tolist()
    else:
        raise NotImplementedError(
            f"Objects of type {type(obj)} are not supported"
        )


def dec_hook(type: Type, obj: Any) -> Any:
    if type is PieceColor:
        return PieceColor(obj)
    if type is PieceType:
        return PieceType(obj)
    if type is np.ndarray:
        return np.array(obj)
    if type is np.int32:
        return np.int32(obj)

    else:
        raise NotImplementedError(f"Objects of type {type} are not supported")


encoder = msgspec.json.Encoder(enc_hook=enc_hook)
decoder = msgspec.json.Decoder(GameSummary, dec_hook=dec_hook)


def import_game_summary(path: Path) -> GameSummary:
    with path.open(mode="rb") as input_file:
        encoded_summary = input_file.read()
    return decoder.decode(encoded_summary)


def export_game_summary(game_summary: GameSummary, path: Path):
    if path.exists():
        raise FileExistsError(f"{path} already exists")
    path.parent.mkdir(parents=True, exist_ok=True)
    encoded_summary = encoder.encode(game_summary)
    with path.open(mode="wb") as output_file:
        output_file.write(encoded_summary)

    print(f"\nSummary data saved to:\n{str(path.resolve())}")

    return path


if __name__ == "__main__":
    my_piece_color = PieceColor.kRed
    my_int = int(my_piece_color)
    print(my_int)
