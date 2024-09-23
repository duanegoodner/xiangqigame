import msgspec
import numpy as np
from datetime import datetime
from pathlib import Path
from typing import Any, Type
from xiangqigame_core import PieceColor, PieceType
from xiangqigame.game import GameSummary


def enc_hook(obj: Any) -> Any:
    if isinstance(obj, PieceColor):
        return int(obj)
    if isinstance(obj, PieceType):
        return int(obj)
    else:
        raise NotImplementedError(
            f"Objects of type {type(obj)} are not supported"
        )


def dec_hook(type: Type, obj: Any) -> Any:
    if type is PieceColor:
        return PieceColor(obj)
    if type is PieceType:
        return PieceType(obj)
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


def export_game_summary(game_summary: GameSummary, path: Path = None) -> Path:
    if path is None:
        time_stamp = "".join(
            char for char in str(datetime.now()) if char.isdigit()
        )
        path = (
            Path(__file__).parent.parent.parent / "data" / f"{time_stamp}.json"
        )

    encoded_summary = encoder.encode(game_summary)
    with path.open(mode="wb") as output_file:
        output_file.write(encoded_summary)
    
    print(f"GameSummary has been saved as {str(path)}")
    
    return path


if __name__ == "__main__":
    my_piece_color = PieceColor.kRed
    my_int = int(my_piece_color)
    print(my_int)
