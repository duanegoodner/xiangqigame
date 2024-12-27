import xiangqi_bindings as xb
import xiangqipy.core_dataclass_mirrors as cdm
import xiangqipy.game_summary as pgs
import xiangqipy.player_summary as pps
import xiangqipy.enums as pen


class CorePlayerSpecToPyPlayerSummaryConverter:
    _move_evaluator_type_converter = {
        xb.EvaluatorType.kHuman: pen.EvaluatorType.NULL,
        xb.EvaluatorType.kRandom: pen.EvaluatorType.RANDOM,
        xb.EvaluatorType.kMinimax: pen.EvaluatorType.MINIMAX,
    }

    def __init__(
        self,
        core_player_spec: xb.PlayerSpec,
        core_search_summaries: xb.SearchSummaries,
    ):
        self.core_player_spec = core_player_spec
        self.core_search_summaries = core_search_summaries

    @property
    def py_color(self) -> xb.PieceColor:
        return self.core_player_spec.color

    @property
    def py_player_type(self) -> pen.PlayerType:
        if self.core_player_spec.evaluator_type == xb.EvaluatorType.kHuman:
            return pen.PlayerType.HUMAN
        else:
            return pen.PlayerType.AI

    @property
    def py_move_evaluator_type(self) -> pen.EvaluatorType:
        return self._move_evaluator_type_converter[
            self.core_player_spec.evaluator_type
        ]

    @property
    def py_max_search_depth(self) -> int:
        return self.core_player_spec.minimax_search_depth

    @property
    def py_zobrist_key_size(self) -> int:
        return self.core_player_spec.zobrist_key_size_bits

    @property
    def py_zkeys_seed(self) -> int | None:
        return None

    @property
    def py_search_summaries(self) -> cdm.SearchSummaries:
        if self.core_search_summaries is not None:
            return cdm.SearchSummaries.from_core_search_summaries(
                core_search_summaries=self.core_search_summaries
            )

    def convert(self) -> pps.PlayerSummary:
        return pps.PlayerSummary(
            color=self.py_color,
            player_type=self.py_player_type,
            move_evaluator_type=self.py_move_evaluator_type,
            max_search_depth=self.py_max_search_depth,
            zobrist_key_size=self.py_zobrist_key_size,
            zkeys_seed=self.py_zkeys_seed,
            search_summaries=self.py_search_summaries,
        )


class CoreToPyGameSummaryConverter:
    _game_state_converter = {
        xb.GameState.kUnfinished: pen.GameState.UNFINISHED,
        xb.GameState.kRedWon: pen.GameState.RED_WON,
        xb.GameState.kBlkWon: pen.GameState.BLACK_WON,
        xb.GameState.kDraw: pen.GameState.DRAW,
    }

    def __init__(self, core_game_summary: xb.GameSummary):
        self.core_game_summary = core_game_summary

    @property
    def py_game_id(self) -> str:
        return self.core_game_summary.game_id

    @property
    def py_game_state(self) -> pen.GameState:
        return self._game_state_converter[self.core_game_summary.game_state]

    @property
    def py_whose_turn(self) -> xb.PieceColor:
        last_move = self.core_game_summary.move_log[-1]
        last_move_piece = last_move.moving_piece
        last_move_color = last_move_piece.piece_color
        return xb.opponent_of(last_move_color)

    @property
    def py_move_log(self) -> list[xb.ExecutedMove]:
        return self.core_game_summary.move_log

    @property
    def py_player_summaries(self) -> pgs.PlayerSummaries:
        return pgs.PlayerSummaries(
            kRed=CorePlayerSpecToPyPlayerSummaryConverter(
                core_player_spec=self.core_game_summary.player_specs[
                    xb.PieceColor.kRed
                ],
                core_search_summaries=self.core_game_summary.search_summaries.get(
                    xb.PieceColor.kRed
                ),
            ).convert(),
            kBlk=CorePlayerSpecToPyPlayerSummaryConverter(
                core_player_spec=self.core_game_summary.player_specs[
                    xb.PieceColor.kBlk
                ],
                core_search_summaries=self.core_game_summary.search_summaries.get(
                    xb.PieceColor.kBlk
                ),
            ).convert(),
        )

    def convert(self) -> pgs.GameSummary:
        return pgs.GameSummary(
            game_id=self.py_game_id,
            game_state=self.py_game_state,
            whose_turn=self.py_whose_turn,
            move_log=self.py_move_log,
            player_summaries=self.py_player_summaries,
        )
