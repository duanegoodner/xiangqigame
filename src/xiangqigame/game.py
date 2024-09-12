from dataclasses import dataclass
from typing import Dict, List

import msgspec
import xiangqigame.terminal_output as msg
from xiangqigame.enums import GameState
from xiangqigame.game_interfaces import (
    GameStatusReporter,
    Player,
    PlayerSummary,
)

from xiangqigame.handlers.errors import handle_interactive_eof
import xiangqigame_core as core
import xiangqigame.core_dataclass_mirrors as cdm


@dataclass
class PlayerSummaries:
    red: PlayerSummary
    black: PlayerSummary


class GameSummary(msgspec.Struct):
    game_state: GameState
    whose_turn: core.PieceColor
    move_log: List[cdm.ExecutedMove]
    player_summaries: PlayerSummaries

    @property
    def move_counts(self) -> int:
        return len(self.move_log)


class Game:

    def __init__(
        self,
        players: Dict[core.PieceColor, Player],
        game_board: core.GameBoard,
        status_reporter: GameStatusReporter = msg.TerminalStatusReporter(),
        move_log: List[core.Move] = None,
    ):
        self._game_state = GameState.UNFINISHED
        self._whose_turn = core.PieceColor.kRed
        self._board = game_board
        self._players = players
        self._status_reporter = status_reporter
        if move_log is None:
            move_log = []
        self._move_log = move_log

    @property
    def summary(self) -> GameSummary:
        return GameSummary(
            game_state=self._game_state,
            whose_turn=self._whose_turn,
            move_log=[
                cdm.ExecutedMove.from_core_executed_move(core_executed_move=item)
                for item in self._move_log
            ],
            player_summaries=PlayerSummaries(
                red=self._players[core.PieceColor.kRed].summary,
                black=self._players[core.PieceColor.kBlk].summary,
            ),
        )

    @property
    def _move_count(self):
        return len(self._move_log)

    def change_whose_turn(self):
        self._whose_turn = core.opponent_of(self._whose_turn)

    @staticmethod
    def is_valid_move(proposed_move: core.Move, avail_moves: List[core.Move]):
        return proposed_move in avail_moves

    def get_valid_move(self, avail_moves: core.MoveCollection):
        valid_move = None
        while not valid_move:
            proposed_move = self._players[self._whose_turn].propose_move(
                self._board, cur_moves=avail_moves
            )
            # TODO move this comparison to a move_calculator.cpp function
            if any(
                [
                    (
                        proposed_move.start.rank == move.start.rank
                        and proposed_move.start.file == move.start.file
                        and proposed_move.end.rank == move.end.rank
                        and proposed_move.end.file == move.end.file
                    )
                    for move in avail_moves.moves
                ]
            ):
                valid_move = proposed_move
            else:
                self._players[self._whose_turn].illegal_move_notice_response(
                    illegal_move=proposed_move,
                    game_board=self._board,
                    cur_moves=avail_moves,
                )
        return valid_move

    def player_turn(self, avail_moves: core.MoveCollection):
        try:
            valid_move = self.get_valid_move(avail_moves=avail_moves)
        except EOFError:
            handle_interactive_eof()
        executed_move = self._board.ExecuteMove(valid_move)
        self._move_log.append(executed_move)

    def set_game_state(self, game_state: GameState):
        self._game_state = game_state

    def set_winner(self, color: int):
        if color == core.PieceColor.kRed:
            self.set_game_state(GameState.RED_WON)
        else:
            self.set_game_state(GameState.BLACK_WON)

    def send_game_info_to_status_reporter(self):
        if self._move_log:
            prev_move = self._move_log[-1].spaces
        else:
            prev_move = None
        self._status_reporter.report_game_info(
            game_state=self._game_state,
            game_board=self._board,
            whose_turn=self._whose_turn,
            is_in_check=self._board.IsInCheck(self._whose_turn),
            move_count=self._move_count,
            prev_move=prev_move,
        )

    def play(self) -> GameSummary:
        while self._game_state == GameState.UNFINISHED:
            self.send_game_info_to_status_reporter()
            avail_moves = self._board.CalcFinalMovesOf(self._whose_turn)
            if avail_moves.size() == 0:
                self.set_winner(core.opponent_of(self._whose_turn))
                break
            self.player_turn(avail_moves=avail_moves)
            self.change_whose_turn()

        self.send_game_info_to_status_reporter()
        return self.summary
