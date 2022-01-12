from common.enums import PieceColor, GameState

team = {
    PieceColor.RED: 'Red',
    PieceColor.BLACK: 'Black'
}

input_prompt = "Enter a move in the form 'from_space, to_space': "
invalid_input = 'Invalid input.'
illegal_move = 'Illegal move. Please enter a different move.'


def display_message(message: str):
    print(message)


def display_object(obj: object):
    print(obj)


def declare_turn_for(color: PieceColor):
    output_string = f"{team[color]} team's turn."
    print(output_string)


def declare_winner(color: PieceColor):
    output_string = f'{team[color]} won the game.'
    print(output_string)
