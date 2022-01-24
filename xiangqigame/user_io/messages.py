from xiangqigame.common.enums import PieceColor, Out, GameState

messages = {
    Out.INPUT_PROMPT: "Enter a move in the form 'from_space, to_space': ",
    Out.INVALID_INPUT: 'Invalid input.',
    Out.ILLEGAL_MOVE: 'Illegal move. Please enter a different move.',
    Out.ILLEGAL_AUTO_MOVE: 'Illegal move in auto play. Move not executed. '
                           'Game ending with no winner.',
    Out.TURN: " team's turn.",
    Out.WON_GAME: ' won the game.',
    Out.IN_CHECK: ' is in check.',
    Out.WHITESPACE: '\n',
    PieceColor.RED: 'Red',
    PieceColor.BLACK: 'Black',
    GameState.UNFINISHED: 'Game has not yet finished.',
    GameState.ILLEGAL_AUTO_MOVE: 'Game ended due to illegal move while playing'
                                 ' in auto mode. There was no winner.',
    GameState.RED_WON: 'Red team won the game.',
    GameState.BLACK_WON: 'Black team won the game.'
}


def output(*args):
    msg_parts = [messages[arg] for arg in args]
    message = ''.join(msg_parts)
    print(message)


def display_object(obj: object):
    print(obj)
