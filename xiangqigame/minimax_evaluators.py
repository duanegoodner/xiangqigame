import abc
import numpy as np
from typing import NamedTuple, Tuple, Dict, List
import xiangqigame.piece_points as pts
from xiangqigame.board_utilities_new import BOARD_UTILITIES as bu
from xiangqigame.game_board_new import GameBoard


class BestMoves(NamedTuple):
    best_eval: int
    best_moves: List[Dict]


class RatedMove(NamedTuple):
    move: Dict
    rating: int


class MinimaxEvaluator(abc.ABC):

    @staticmethod
    def evaluate_winner(cur_player: int, initiating_player: int):
        if cur_player == initiating_player:
            return BestMoves(best_eval=-np.inf, best_moves=[])
        else:
            return BestMoves(best_eval=np.inf, best_moves=[])

    @abc.abstractmethod
    def evaluate_leaf(
            self,
            game_board: GameBoard,
            cur_player: int,
            cur_player_moves: List[Dict],
            initiating_player: int) -> BestMoves:
        pass

    @abc.abstractmethod
    def rate_move(
            self,
            move: Dict,
            game_board: GameBoard,
            cur_player: int) -> RatedMove:
        pass

    def generate_ranked_move_list(
            self,
            game_board: GameBoard,
            cur_player: int,
            cur_player_moves: List[Dict]):
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
            cur_player: int,
            cur_player_moves: List[Dict],
            initiating_player: int) -> BestMoves:
        updated_opponent_moves = game_board.calc_final_moves_of(
            color=bu.opponent_of[cur_player])
        if cur_player == initiating_player:
            return BestMoves(
                best_eval=len(cur_player_moves) - len(updated_opponent_moves),
                best_moves=[])
        else:
            return BestMoves(
                best_eval=len(updated_opponent_moves) - len(cur_player_moves),
                best_moves=[])

    def rate_move(
            self,
            move: Dict,
            game_board: GameBoard,
            cur_player: int):
        if game_board.get_color(move["end"]) == bu.opponent_of[cur_player]:
            return RatedMove(move=move, rating=1)
        else:
            return RatedMove(move=move, rating=0)


class PiecePoints(MinimaxEvaluator):

    def __init__(self,
                 base_pts: pts.BasePoints,
                 position_pts: pts.PositionPts,
                 position_multiplier: int = 1,

                 ):
        self._base_pts = base_pts
        self._position_pts = position_pts
        self._position_multiplier = position_multiplier

    def get_value_of_piece_at_position(
            self,
            color: int,
            piece_type: int,
            space: Tuple[int, int]):
        return (self._base_pts.vals[piece_type] + self._position_multiplier *
                self._position_pts.vals[color][piece_type][
                    space[0], space[1]])

    def get_player_total(self, color: int, game_board: GameBoard):
        pre_attack_total = 0
        for space in game_board.get_all_spaces_occupied_by(color):
            piece_type = game_board.get_type(space)
            pre_attack_total += self.get_value_of_piece_at_position(
                color=color, piece_type=piece_type, space=space)
        return pre_attack_total

    def evaluate_leaf(
            self,
            game_board: GameBoard,
            cur_player: int,
            cur_player_moves: List[Dict],
            initiating_player: int) -> BestMoves:
        cur_player_pts = self.get_player_total(cur_player, game_board)
        opponent_pts = self.get_player_total(bu.opponent_of[cur_player],
                                             game_board)

        if cur_player == initiating_player:
            return BestMoves(
                best_eval=cur_player_pts - opponent_pts,
                best_moves=[])
        else:
            return BestMoves(
                best_eval=opponent_pts - cur_player_pts,
                best_moves=[])

    def rate_move(
            self,
            move: Dict,
            game_board: GameBoard,
            cur_player: int) -> RatedMove:

        piece_type = game_board.get_type(move["start"])
        cur_player_position_array = self._position_pts.vals[cur_player][
            piece_type]
        position_value_delta = self._position_multiplier * (
                cur_player_position_array[move["end"][0], move["end"][1]] -
                cur_player_position_array[move["start"][0], move["start"][1]]
        )

        if game_board.get_color(move["end"]) == bu.opponent_of[cur_player]:
            captured_piece_type = game_board.get_type(move["end"])
            opponent_position_array = self._position_pts.vals[
                bu.opponent_of[cur_player]][captured_piece_type]
            capture_val = (
                    self._base_pts.vals[captured_piece_type] +
                    self._position_multiplier *
                    opponent_position_array[move["end"][0], move["end"][1]])
        else:
            capture_val = 0

        return RatedMove(move=move, rating=position_value_delta + capture_val)
