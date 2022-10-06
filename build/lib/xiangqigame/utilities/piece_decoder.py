import colorama as cr
from typing import Dict
from xiangqigame.piece_definitions import PColor, PType
from xiangqigame.game_piece import GamePiece


_disp_format = {
    PColor.RED: cr.Fore.RED + cr.Back.WHITE,
    PColor.BLK: cr.Fore.BLACK + cr.Back.WHITE,
    PColor.NUL: cr.Fore.RESET + cr.Back.RESET
}


def encode_piece(piece_info: Dict):
    return (
        f"{_disp_format[piece_info['color']]}"
        f"{_type_to_code[piece_info['piece_type']]}"
        f"{_color_to_code[piece_info['color']]}"
        f"{cr.Style.RESET_ALL}"
    )


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



