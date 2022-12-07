from typing import Dict, List
import xiangqigame.terminal_output as msg
from GameBoardPy import GameBoard, Move, opponent_of, PieceColor
from xiangqigame.enums import GameState
from xiangqigame.game_interfaces import GameStatusReporter
from xiangqigame.players import Player
from xiangqigame.handlers.errors import handle_interactive_eof


class Game:

    def __init__(
            self,
            players: Dict[PieceColor, Player],
            # red_player: Player,
            # black_player: Player,
            status_reporter: GameStatusReporter = msg.TerminalStatusReporter(),
            move_log: List[Move] = None):
        self._game_state = GameState.UNFINISHED
        self._whose_turn = PieceColor.kRed
        self._board = GameBoard()
        self._players = players
        self._status_reporter = status_reporter
        if move_log is None:
            move_log = []
        self._move_log = move_log

    @property
    def _move_count(self):
        return len(self._move_log)

    def change_whose_turn(self):
        self._whose_turn = opponent_of(self._whose_turn)

    @staticmethod
    def is_valid_move(proposed_move: Move, avail_moves: List[Move]):
        return proposed_move in avail_moves

    def get_valid_move(self, avail_moves: List[Move]):
        valid_move = None
        while not valid_move:
            proposed_move = self._players[self._whose_turn].propose_move(
                self._board, cur_moves=avail_moves)
            # TODO move this comparison to a move_calculator.cpp function
            if any([(
                    proposed_move.start.rank == move.start.rank and
                    proposed_move.start.file == move.start.file and
                    proposed_move.end.rank == move.end.rank and
                    proposed_move.end.file == move.end.file
            ) for move in avail_moves]):
                valid_move = proposed_move
            else:
                self._players[self._whose_turn].illegal_move_notice_response(
                    illegal_move=proposed_move,
                    game_board=self._board,
                    cur_moves=avail_moves)
        return valid_move

    def player_turn(self, avail_moves: List[Move]):
        try:
            valid_move = self.get_valid_move(avail_moves=avail_moves)
        except EOFError:
            handle_interactive_eof()
        self._board.ExecuteMove(valid_move)
        self._move_log.append(valid_move)

    def set_game_state(self, game_state: GameState):
        self._game_state = game_state

    def set_winner(self, color: int):
        if color == PieceColor.kRed:
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
            is_in_check=self._board.IsInCheck(self._whose_turn),
            move_count=self._move_count,
            prev_move=prev_move
        )

    def play(self):
        while self._game_state == GameState.UNFINISHED:
            self.send_game_info_to_status_reporter()
            avail_moves = self._board.CalcFinalMovesOf(self._whose_turn)
            if not avail_moves:
                self.set_winner(opponent_of(self._whose_turn))
                break
            self.player_turn(avail_moves=avail_moves)
            self.change_whose_turn()

        self.send_game_info_to_status_reporter()
