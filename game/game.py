from common.enums import GameState, PieceColor
from board.game_board import GameBoard
from common.game_rules import opponent_of
from user_io.interactive import get_input_indices


def get_user_input():
    valid_input = None
    while valid_input is None:
        user_input = input("Enter move in form ((from.rank, from.file),"
                           "(to.rank, to.file))")

        print(user_input)
        if (not isinstance(user_input, tuple)) or (len(user_input) != 2) or \
                (not all(isinstance(space, tuple) for space in user_input)) or\
                (not all(len(space) == 2 for space in user_input)):
            continue

        valid_input = user_input
    return valid_input


class Game:

    def __init__(self, game_config):
        board_data = game_config['board_data']

        self._game_state = GameState.UNFINISHED
        self._whose_turn = PieceColor.RED
        self._board = GameBoard(board_data)
        self._moves = {
            PieceColor.RED: self._board.calc_final_moves_of(PieceColor.RED),
            PieceColor.BLACK: self._board.calc_final_moves_of(PieceColor.BLACK)
        }

    def is_in_check(self, color: PieceColor):
        opp_destinations = {
            move[1] for move in self._moves[opponent_of[color]]}
        return self._board.get_general_position(color) in opp_destinations

    # def is_valid_move(self, from_space: BoardSpace, to_space: BoardSpace):
    #
    #     if not bu.is_on_board(from_space):
    #         print("Cannot move from a space that is not on board.")
    #         return False
    #     if not bu.is_on_board(to_space):
    #         print("Cannot move to a space that is not on board.")
    #         return False
    #     if self._board.get_color(from_space) != self._whose_turn or \
    #             (from_space, to_space) not in self._moves[self._whose_turn]:
    #         print("Invalid move.")
    #         return False
    #     return True

    def play(self):
        pass

    def change_whose_turn(self):
        self._whose_turn = opponent_of[self._whose_turn]
