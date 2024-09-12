import msgspec
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
    else:
        raise NotImplementedError(
        f"Objects of type {type} are not supported"
    )

enc = msgspec.json.Encoder(enc_hook=enc_hook)
dec = msgspec.json.Decoder(GameSummary, dec_hook=dec_hook)


if __name__ == "__main__":
    my_piece_color = PieceColor.kRed
    my_int = int(my_piece_color)
    print(my_int)
