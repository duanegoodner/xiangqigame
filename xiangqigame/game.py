from typing import Dict
import xiangqigame.user_io.messages as msg
from xiangqigame.board_rules import BOARD_RULES as br
from xiangqigame.enums import GameState, PieceColor, Out
from xiangqigame.game_board import GameBoard
from xiangqigame.players import Player
from xiangqigame.move import Move
from xiangqigame.user_io.display import clear_screen

from xiangqigame.handlers.errors import handle_interactive_eof


class Game:

    def __init__(
            self,
            game_config: Dict,
            red_player: Player,
            black_player: Player,
            scripted_moves=None):
        self._game_state = GameState.UNFINISHED
        self._whose_turn = PieceColor.RED
        self._board = GameBoard(game_config['board_data'])
        self._players = {PieceColor.RED: red_player,
                         PieceColor.BLACK: black_player}

        self._moves = {
            PieceColor.RED: self._board.calc_final_moves_of(PieceColor.RED),
            PieceColor.BLACK: self._board.calc_final_moves_of(PieceColor.BLACK)
        }
        self._scripted_moves = scripted_moves
        self._scripted_move_idx = 0

    def is_in_check(self, color: PieceColor):
        opp_destinations = {
            move[1] for move in self._moves[br.opponent_of[color]]}
        return self._board.get_general_position(color) in opp_destinations

    def change_whose_turn(self):
        self._whose_turn = br.opponent_of[self._whose_turn]

    def is_valid_move(self, proposed_move: Move):
        return proposed_move in self._moves[self._whose_turn]

    def get_valid_move(self):
        valid_move = None
        while not valid_move:
            proposed_move = self._players[self._whose_turn].propose_move(
                self._board, cur_moves=self._moves[self._whose_turn])
            if proposed_move in self._moves[self._whose_turn]:
                valid_move = proposed_move
            else:
                self._players[self._whose_turn].illegal_move_notice_response(
                    self._board, cur_moves=self._moves[self._whose_turn])
        return valid_move

    def player_turn(self):
        clear_screen()
        msg.display_object(self._board)
        msg.output(Out.WHITESPACE)
        msg.output(Out.TURN, self._whose_turn)
        msg.output(Out.WHITESPACE)

        if self._players[br.opponent_of[self._whose_turn]].move_log:
            print(self._players[br.opponent_of[self._whose_turn]].move_log[-1])

        if self.is_in_check(self._whose_turn):
            msg.output(self._whose_turn, Out.IN_CHECK)
        try:
            valid_move = self.get_valid_move()
        except EOFError:
            handle_interactive_eof()
        self._board.execute_move(valid_move)
        # clear_screen()
        # msg.display_object(self._board)
        # msg.output(Out.WHITESPACE)

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

    def play(self):
        # clear_screen()
        msg.display_object(self._board)
        while self._game_state == GameState.UNFINISHED:
            self.player_turn()
            self.update_moves()
            self.change_whose_turn()
            if self._moves[self._whose_turn] == set():
                self.set_winner(br.opponent_of[self._whose_turn])

        # msg.output(Out.WHITESPACE)
        msg.output(br.opponent_of[self._whose_turn], Out.WON_GAME)
