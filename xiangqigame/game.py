from typing import Dict, List
import xiangqigame.terminal_output as msg
from xiangqigame.board_utilities_new import BOARD_UTILITIES as bu
from xiangqigame.enums import GameState
from xiangqigame.piece_definitions import PColor
from xiangqigame.game_board_new import GameBoard
from xiangqigame.game_interfaces import GameStatusReporter
from xiangqigame.players import Player

from xiangqigame.handlers.errors import handle_interactive_eof


class Game:

    def __init__(
            self,
            # game_config: Dict,
            red_player: Player,
            black_player: Player,
            status_reporter: GameStatusReporter = msg.TerminalStatusReporter(),
            move_log: List[Dict] = None):
        self._game_state = GameState.UNFINISHED
        self._whose_turn = PColor.RED
        self._board = GameBoard()
        self._players = {PColor.RED: red_player,
                         PColor.BLK: black_player}
        self._status_reporter = status_reporter
        if move_log is None:
            move_log = []
        self._move_log = move_log
        # self._winner = None

    @property
    def _move_count(self):
        return len(self._move_log)

    def is_in_check(self, color: int):
        opp_destinations = {
            move["end"] for move in self._board.calc_final_moves_of(
                bu.opponent_of[color])}
        return self._board.get_general_position(color) in opp_destinations

    def change_whose_turn(self):
        self._whose_turn = bu.opponent_of[self._whose_turn]

    def is_valid_move(self, proposed_move: Dict):
        return proposed_move in self._board.calc_final_moves_of(
            self._whose_turn)

    def get_valid_move(self):
        valid_move = None
        while not valid_move:
            proposed_move = self._players[self._whose_turn].propose_move(
                self._board, cur_moves=self._board.calc_final_moves_of(
                    self._whose_turn))
            if proposed_move in self._board.calc_final_moves_of(
                    self._whose_turn):
                valid_move = proposed_move
            else:
                self._players[self._whose_turn].illegal_move_notice_response(
                    illegal_move=proposed_move,
                    game_board=self._board,
                    cur_moves=self._board.calc_final_moves_of(self._whose_turn))
        return valid_move

    def player_turn(self):
        try:
            valid_move = self.get_valid_move()
        except EOFError:
            handle_interactive_eof()
        self._board.execute_move(valid_move)
        self._move_log.append(valid_move)

    def set_game_state(self, game_state: GameState):
        self._game_state = game_state

    def set_winner(self, color: int):
        if color == PColor.RED:
            self.set_game_state(GameState.RED_WON)
        else:
            self.set_game_state(GameState.BLACK_WON)

    def send_game_info_to_status_reporter(self):
        if self._move_log:
            prev_move = self._move_log[-1]
        else:
            prev_move = None
        self._status_reporter.report_game_info(
            game_state=self._game_state,
            game_board=self._board,
            whose_turn=self._whose_turn,
            is_in_check=self.is_in_check(self._whose_turn),
            move_count=self._move_count,
            prev_move=prev_move
        )

    def play(self):
        while self._game_state == GameState.UNFINISHED:
            self.send_game_info_to_status_reporter()
            self.player_turn()
            self.change_whose_turn()
            if not self._board.calc_final_moves_of(self._whose_turn):
                self.set_winner(bu.opponent_of[self._whose_turn])

        self.send_game_info_to_status_reporter()
