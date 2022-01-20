from xiangqi.common.enums import GameState, PieceColor, Out
from xiangqi.board.game_board import GameBoard
from xiangqi.board.move import Move
from xiangqi.user_io.single_move import get_proposed_move
from xiangqi.user_io.display import clear_screen
import xiangqi.user_io.messages as msg


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
            move[1] for move in self._moves[self._board.opponent_of[color]]}
        return self._board.get_general_position(color) in opp_destinations

    def change_whose_turn(self):
        self._whose_turn = self._board.opponent_of[self._whose_turn]

    def is_valid_move(self, proposed_move: Move):
        return proposed_move in self._moves[self._whose_turn]

    def get_valid_move(self):
        valid_move = None
        while not valid_move:
            proposed_move = get_proposed_move()
            if proposed_move in self._moves[self._whose_turn]:
                valid_move = proposed_move
            else:
                msg.output(Out.ILLEGAL_MOVE)
        return valid_move

    def player_turn(self):
        msg.output(self._whose_turn, Out.TURN)
        if self.is_in_check(self._whose_turn):
            msg.output(self._whose_turn, Out.IN_CHECK)
        valid_move = self.get_valid_move()
        self._board.execute_move(valid_move)
        clear_screen()
        msg.display_object(self._board)
        # msg.output(Out.WHITESPACE)

    def auto_player_turn(self):
        msg.output(self._whose_turn, Out.TURN)
        if self.is_in_check(self._whose_turn):
            msg.output(self._whose_turn, Out.IN_CHECK)
        cur_move = self._auto_moves[self._auto_move_idx]
        msg.display_object(cur_move)
        if not self.is_valid_move(cur_move):
            msg.output(Out.ILLEGAL_AUTO_MOVE)
            msg.display_object(self._board)
            self.set_game_state(GameState.ILLEGAL_AUTO_MOVE)
            return
        self._board.execute_move(cur_move)
        msg.display_object(self._board)
        msg.output(Out.WHITESPACE)

    def update_moves(self):
        self._moves[PieceColor.RED] = self._board.calc_final_moves_of(
            PieceColor.RED)
        self._moves[PieceColor.BLACK] = self._board.calc_final_moves_of(
            PieceColor.BLACK)

    def set_game_state(self, game_state: GameState):
        self._game_state = game_state

    def set_winner(self, color: PieceColor):
        if color == PieceColor.RED:
            self.set_game_state(GameState.RED_WON)
        else:
            self.set_game_state(GameState.BLACK_WON)

    def play_interactive(self):
        clear_screen()
        msg.display_object(self._board)
        while self._game_state == GameState.UNFINISHED:
            self.player_turn()
            self.update_moves()
            self.change_whose_turn()
            if self._moves[self._whose_turn] == set():
                self.set_winner(self._board.opponent_of[self._whose_turn])

        msg.output(self._board.opponent_of[self._whose_turn], Out.WON_GAME)

    def play_auto_moves(self):
        clear_screen()
        msg.display_object(self._board)
        while self._game_state == GameState.UNFINISHED and self._auto_move_idx\
                < len(self._auto_moves):
            self.auto_player_turn()
            self.update_moves()
            self.change_whose_turn()
            self._auto_move_idx += 1
            if self._moves[self._whose_turn] == set():
                self.set_winner(self._board.opponent_of[self._whose_turn])

        msg.output(self._game_state)









