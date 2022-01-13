from common.enums import GameState, PieceColor
from board.game_board import GameBoard
from common.game_rules import opponent_of
from board.move import Move
from user_io.single_move import get_proposed_move
import user_io.messages as msg



class Game:

    def __init__(self, game_config, auto_moves=None):
        board_data = game_config['board_data']

        self._game_state = GameState.UNFINISHED
        self._whose_turn = PieceColor.RED
        self._board = GameBoard(board_data)
        self._moves = {
            PieceColor.RED: self._board.calc_final_moves_of(PieceColor.RED),
            PieceColor.BLACK: self._board.calc_final_moves_of(PieceColor.BLACK)
        }
        self._auto_moves = auto_moves
        self._auto_move_idx = 0

    def is_in_check(self, color: PieceColor):
        opp_destinations = {
            move[1] for move in self._moves[opponent_of[color]]}
        return self._board.get_general_position(color) in opp_destinations

    def change_whose_turn(self):
        self._whose_turn = opponent_of[self._whose_turn]

    def is_valid_move(self, proposed_move: Move):
        return proposed_move in self._moves[self._whose_turn]

    def get_valid_move(self):
        valid_move = None
        while not valid_move:
            proposed_move = get_proposed_move()
            if proposed_move in self._moves[self._whose_turn]:
                valid_move = proposed_move
            else:
                msg.display_message(msg.illegal_move)
        return valid_move

    def player_turn(self):
        msg.declare_turn_for(self._whose_turn)
        valid_move = self.get_valid_move()
        self._board.execute_move(valid_move)
        msg.display_object(self._board)

    def auto_player_turn(self):
        msg.declare_turn_for(self._whose_turn)
        if self.is_in_check(self._whose_turn):
            msg.declare_in_check(self._whose_turn)
        cur_move = self._auto_moves[self._auto_move_idx]
        msg.display_object(cur_move)
        if not self.is_valid_move(cur_move):
            msg.display_message('ILLEGAL MOVE. NOT EXECUTED ****************')
            msg.display_object(self._board)
            return
        self._board.execute_move(cur_move)
        msg.display_object(self._board)

    def update_moves(self):
        self._moves[self._whose_turn] = self._board.calc_final_moves_of(
            self._whose_turn)

    def set_winner(self, color: PieceColor):
        if color == PieceColor.RED:
            self._game_state = GameState.RED_WON
        else:
            self._game_state = PieceColor.BLACK

    def play_interactive(self):
        msg.display_object(self._board)
        while self._game_state == GameState.UNFINISHED:
            self.player_turn()
            self.change_whose_turn()
            self.update_moves()
            if self._moves[self._whose_turn] == set():
                self.set_winner(opponent_of[self._whose_turn])

        msg.declare_winner(opponent_of[self._whose_turn])

    def play_auto_moves(self):
        msg.display_object(self._board)
        while self._game_state == GameState.UNFINISHED and self._auto_move_idx\
                < len(self._auto_moves):
            self.auto_player_turn()
            self.update_moves()
            self.change_whose_turn()
            self._auto_move_idx += 1
            self.update_moves()
            if self._moves[self._whose_turn] == set():
                self.set_winner(opponent_of[self._whose_turn])

        print(self._game_state)









