from typing import Dict
from xiangqigame.piece_definitions import PColor, PType
from xiangqigame.game_piece import GamePiece

# Used for decoding/encoding board data provided to Game in following format.
# (No longer used, may start using again if add ability to pause/resume games)
# {
#   "board_data": [
#     ["Rb", "Hb", "Eb", "Ab", "Gb", "Ab", "Eb", "Hb", "Rb"],
#     ["--", "--", "--", "--", "--", "--", "--", "--", "--"],
#     ["--", "Cb", "--", "--", "--", "--", "--", "Cb", "--"],
#     ["Sb", "--", "Sb", "--", "Sb", "--", "Sb", "--", "Sb"],
#     ["--", "--", "--", "--", "--", "--", "--", "--", "--"],
#     ["--", "--", "--", "--", "--", "--", "--", "--", "--"],
#     ["Sr", "--", "Sr", "--", "Sr", "--", "Sr", "--", "Sr"],
#     ["--", "Cr", "--", "--", "--", "--", "--", "Cr", "--"],
#     ["--", "--", "--", "--", "--", "--", "--", "--", "--"],
#     ["Rr", "Hr", "Er", "Ar", "Gr", "Ar", "Er", "Hr", "Rr"]
#   ]
#
# }


_code_to_color = {
    'r': PColor.RED,
    'b': PColor.BLK,
    '-': PColor.NUL
}


_color_to_code = dict((color, code) for code, color in
                      _code_to_color.items())

_code_to_type = {
    'R': PType.CHA,
    'H': PType.HOR,
    'E': PType.ELE,
    'A': PType.ADV,
    'G': PType.GEN,
    'C': PType.CAN,
    'S': PType.SOL,
    '-': PType.NUL
}

_type_to_code = dict(
    (color, code) for code, color in _code_to_type.items())


def decode_color(piece_code):
    return _code_to_color[piece_code[1]]


def decode_piece_type(piece_code):
    return _code_to_type[piece_code[0]]


def decode_piece(piece_code):
    decoded_piece: GamePiece = {
        'type': _code_to_type[piece_code[0]],
        'color': _code_to_color[piece_code[1]]
    }
    return decoded_piece


def encode_piece(piece_info: Dict):
    return (
        f"{_type_to_code[piece_info['piece_type']]}"
        f"{_color_to_code[piece_info['color']]}"
    )



