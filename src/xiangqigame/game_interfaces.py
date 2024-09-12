import abc
from dataclasses import dataclass
from xiangqigame_core import GameBoard, Move, PieceColor, SearchSummaries
from xiangqigame.binding_dataclasses import SearchSummariesD
from typing import List
from xiangqigame.enums import GameState


@dataclass
class PlayerSummaryD:
    player_type: str
    move_evaluator_type: str = None
    max_search_depth: int = None
    zobrist_key_size: int = None
    search_summaries: SearchSummariesD = None


class Player(abc.ABC):

    def __init__(self, color: PieceColor):
        self._color = color

    @abc.abstractmethod
    def propose_move(
        self, game_board: GameBoard, cur_moves: List[Move]
    ) -> Move:
        pass

    @abc.abstractmethod
    def illegal_move_notice_response(
        self, illegal_move: Move, game_board: GameBoard, cur_moves: List[Move]
    ):
        pass

    @property
    def player_type(self) -> str:
        return type(self).__name__

    @property
    def move_evaluator_type(self) -> str | None:
        if hasattr(self, "_move_evaluator"):
            return type(self._move_evaluator).__name__

    @property
    def max_search_depth(self) -> int | None:
        if self.move_evaluator_type in [
            "MinimaxMoveEvaluator64",
            "MinimaxMoveEvaluator128",
        ]:
            return self._move_evaluator.starting_search_depth()

    @property
    def zobrist_key_size(self) -> int | None:
        if self.move_evaluator_type in [
            "MinimaxMoveEvaluator64",
            "MinimaxMoveEvaluator128",
        ]:
            return self._move_evaluator.zobrist_key_size_bits()

    @property
    def search_summaries(self) -> SearchSummariesD | None:
        if self.move_evaluator_type in [
            "MinimaxMoveEvaluator64",
            "MinimaxMoveEvaluator128",
        ]:
            return SearchSummariesD.from_core_search_summaries(
                core_search_summaries=self._move_evaluator.get_search_summaries()
            )

    @property
    def summary(self) -> PlayerSummaryD:
        return PlayerSummaryD(
            player_type=self.player_type,
            move_evaluator_type=self.move_evaluator_type,
            max_search_depth=self.max_search_depth,
            zobrist_key_size=self.zobrist_key_size,
            search_summaries=self.search_summaries,
        )


class GameStatusReporter(abc.ABC):

    @abc.abstractmethod
    def report_game_info(
        self,
        game_state: GameState,
        game_board: GameBoard,
        whose_turn: PieceColor,
        is_in_check: bool,
        move_count: int,
        prev_move: Move = None,
    ):
        pass
