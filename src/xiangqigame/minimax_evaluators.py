import abc
import numpy as np
from xiangqigame_cpp.xiangqigame_core import (
    BoardSpace,
    GameBoard,
    Move,
    opponent_of,
    PieceColor,
    PieceType,
)
from typing import NamedTuple, List
import xiangqigame.piece_points as pts


class BestMoves(NamedTuple):
    best_eval: int
    best_moves: List[Move]


class RatedMove(NamedTuple):
    move: Move
    rating: int


class MinimaxEvaluator(abc.ABC):

    @staticmethod
    def evaluate_winner(cur_player: PieceColor, initiating_player: PieceColor):
        if cur_player == initiating_player:
            return BestMoves(best_eval=-np.inf, best_moves=[])
        else:
            return BestMoves(best_eval=np.inf, best_moves=[])

    @abc.abstractmethod
    def evaluate_leaf(
        self,
        game_board: GameBoard,
        cur_player: PieceColor,
        cur_player_moves: List[Move],
        initiating_player: PieceColor,
    ) -> BestMoves:
        pass

    @abc.abstractmethod
    def rate_move(
        self, move: Move, game_board: GameBoard, cur_player: PieceColor
    ) -> RatedMove:
        pass

    def generate_ranked_move_list(
        self,
        game_board: GameBoard,
        cur_player: PieceColor,
        cur_player_moves: List[Move],
    ):
        move_list = [
            self.rate_move(move=move, game_board=game_board, cur_player=cur_player)
            for move in cur_player_moves
        ]
        move_list.sort(key=lambda x: x.rating, reverse=True)
        return move_list


class MoveCounts(MinimaxEvaluator):

    def evaluate_leaf(
        self,
        game_board: GameBoard,
        cur_player: PieceColor,
        cur_player_moves: List[Move],
        initiating_player: PieceColor,
    ) -> BestMoves:
        updated_opponent_moves = game_board.CalcFinalMovesOf(
            color=opponent_of(cur_player)
        )
        if cur_player == initiating_player:
            return BestMoves(
                best_eval=len(cur_player_moves) - len(updated_opponent_moves),
                best_moves=[],
            )
        else:
            return BestMoves(
                best_eval=len(updated_opponent_moves) - len(cur_player_moves),
                best_moves=[],
            )

    def rate_move(self, move: Move, game_board: GameBoard, cur_player: PieceColor):
        if game_board.GetColor(move.end) == opponent_of(cur_player):
            return RatedMove(move=move, rating=1)
        else:
            return RatedMove(move=move, rating=0)


class PiecePoints(MinimaxEvaluator):

    def __init__(
        self,
        base_pts: pts.BasePoints,
        position_pts: pts.PositionPts,
        position_multiplier: int = 1,
    ):
        self._base_pts = base_pts
        self._position_pts = position_pts
        self._position_multiplier = position_multiplier

    def get_value_of_piece_at_position(
        self, color: PieceColor, piece_type: PieceType, space: BoardSpace
    ):
        return (
            self._base_pts.vals[piece_type]
            + self._position_multiplier
            * self._position_pts.vals[color][piece_type][space.rank, space.file]
        )

    def get_player_total(self, color: PieceColor, game_board: GameBoard):
        pre_attack_total = 0
        for space in game_board.GetAllSpacesOccupiedBy(color):
            piece_type = game_board.GetType(space)
            pre_attack_total += self.get_value_of_piece_at_position(
                color=color, piece_type=piece_type, space=space
            )
        return pre_attack_total

    def evaluate_leaf(
        self,
        game_board: GameBoard,
        cur_player: PieceColor,
        cur_player_moves: List[Move],
        initiating_player: PieceColor,
    ) -> BestMoves:
        cur_player_pts = self.get_player_total(cur_player, game_board)
        opponent_pts = self.get_player_total(opponent_of(cur_player), game_board)

        if cur_player == initiating_player:
            return BestMoves(best_eval=cur_player_pts - opponent_pts, best_moves=[])
        else:
            return BestMoves(best_eval=opponent_pts - cur_player_pts, best_moves=[])

    def rate_move(
        self, move: Move, game_board: GameBoard, cur_player: int
    ) -> RatedMove:

        piece_type = game_board.GetType(move.start)
        cur_player_position_array = self._position_pts.vals[cur_player][piece_type]
        position_value_delta = self._position_multiplier * (
            cur_player_position_array[move.end.rank, move.end.file]
            - cur_player_position_array[move.start.rank, move.start.file]
        )

        if game_board.GetColor(move.end) == opponent_of(cur_player):
            captured_piece_type = game_board.GetType(move.end)
            opponent_position_array = self._position_pts.vals[opponent_of(cur_player)][
                captured_piece_type
            ]
            capture_val = (
                self._base_pts.vals[captured_piece_type]
                + self._position_multiplier
                * opponent_position_array[move.end.rank, move.end.file]
            )
        else:
            capture_val = 0

        return RatedMove(move=move, rating=position_value_delta + capture_val)


DEFAULT_MINIMAX_EVALUATOR = PiecePoints(
    base_pts=pts.DEFAULT_BASE_POINTS,
    position_pts=pts.DEFAULT_POSITION_POINTS,
    position_multiplier=1,
)
