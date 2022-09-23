import abc
import numpy as np
from typing import Set, NamedTuple
import xiangqigame.piece_points as pts
from xiangqigame.board_rules import BOARD_RULES as br
from xiangqigame.move import Move
from xiangqigame.game_board import GameBoard
from xiangqigame.enums import PieceColor


class BestMoves(NamedTuple):
    best_eval: int
    best_moves: Set[Move]


class RatedMove(NamedTuple):
    move: Move
    rating: int


class MinimaxEvaluator(abc.ABC):

    @staticmethod
    def evaluate_winner(cur_player: PieceColor, initiating_player: PieceColor):
        if cur_player == initiating_player:
            return BestMoves(best_eval=-np.inf, best_moves=set())
        else:
            return BestMoves(best_eval=np.inf, best_moves=set())

    @abc.abstractmethod
    def evaluate_leaf(
            self,
            game_board: GameBoard,
            cur_player: PieceColor,
            cur_player_moves: Set[Move],
            initiating_player: PieceColor) -> BestMoves:
        pass

    @abc.abstractmethod
    def rate_move(
            self,
            move: Move,
            game_board: GameBoard,
            cur_player: PieceColor) -> RatedMove:
        pass

    def generate_ranked_move_list(
            self,
            game_board: GameBoard,
            cur_player: PieceColor,
            cur_player_moves: Set[Move]):
        move_list = [
            self.rate_move(
                move=move, game_board=game_board, cur_player=cur_player) for
            move in cur_player_moves]
        move_list.sort(key=lambda x: x.rating, reverse=True)
        return move_list


class MoveCounts(MinimaxEvaluator):

    def evaluate_leaf(
            self,
            game_board: GameBoard,
            cur_player: PieceColor,
            cur_player_moves: Set[Move],
            initiating_player: PieceColor) -> BestMoves:
        opponent_moves = game_board.calc_final_moves_of(
            color=br.opponent_of[cur_player])
        if cur_player == initiating_player:
            return BestMoves(
                best_eval=len(cur_player_moves) - len(opponent_moves),
                best_moves=set())
        else:
            return BestMoves(
                best_eval=len(opponent_moves) - len(cur_player_moves),
                best_moves=set())

    def rate_move(
            self,
            move: Move,
            game_board: GameBoard,
            cur_player: PieceColor):
        if game_board.get_color(move.end) == br.opponent_of[cur_player]:
            return RatedMove(move=move, rating=1)
        else:
            return RatedMove(move=move, rating=0)


class PiecePoints(MinimaxEvaluator):

    def __init__(self, position_multiplier: int = 1):
        self._position_multiplier = position_multiplier

    def get_point_total(self, color: PieceColor, game_board: GameBoard):
        player_total = 0
        for space in game_board.get_all_spaces_occupied_by(color):
            piece_type = game_board.get_type(space)
            player_total += (
                    pts.base_points[piece_type] + self._position_multiplier *
                    pts.position_points[piece_type][space.rank, space.file])
        return player_total

    def evaluate_leaf(
            self,
            game_board: GameBoard,
            cur_player: PieceColor,
            cur_player_moves: Set[Move],
            initiating_player: PieceColor) -> BestMoves:
        cur_player_pts = self.get_point_total(cur_player, game_board)
        opponent_pts = self.get_point_total(br.opponent_of[cur_player],
                                            game_board)

        if cur_player == initiating_player:
            return BestMoves(
                best_eval=cur_player_pts - opponent_pts,
                best_moves=set())
        else:
            return BestMoves(
                best_eval=opponent_pts - cur_player_pts,
                best_moves=set())

    def rate_move(
            self,
            move: Move,
            game_board: GameBoard,
            cur_player: PieceColor) -> RatedMove:
        piece_type = game_board.get_type(move.start)
        position_pts_array = pts.position_points[piece_type]
        position_value_delta = self._position_multiplier * (
            position_pts_array[move.end.rank, move.end.file] -
            position_pts_array[move.start.rank, move.start.file]
        )

        if game_board.get_color(move.end) == br.opponent_of[cur_player]:
            captured_piece_type = game_board.get_type(move.end)
            capture_val = (
                    pts.base_points[captured_piece_type] +
                    self._position_multiplier *
                    pts.position_points[captured_piece_type]
                    [move.end.rank, move.end.file])
        else:
            capture_val = 0

        return RatedMove(move=move, rating=position_value_delta + capture_val)
