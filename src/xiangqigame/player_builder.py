# from cpp_modules.src.pybind_modules.GameBoardPy import PieceColor
from xiangqigame.command_input import XiangqiGameCommand, XiangqiGameCommandLine, PlayerInput
# from xiangqigame.move_selectors import MinimaxMoveSelector, RandomMoveSelector
from xiangqigame_cpp.xiangqigame_core import MinimaxMoveSelectorPy, PieceColor, RandomMoveSelector
from xiangqigame.players import AIPlayer, HumanPlayer
from xiangqigame.minimax_evaluators import DEFAULT_MINIMAX_EVALUATOR


class SinglePlayerBuilder:

    _default_player_strengths = {
        PieceColor.kRed: 2,
        PieceColor.kBlk: 3
    }

    def __init__(
            self,
            player_input: PlayerInput,
            color: PieceColor,
    ):

        self._color = color
        if player_input.player_type is None:
            player_input.player_type = AIPlayer
        self._player_constructor = player_input.player_type
        self._move_selector_constructor = player_input.algo
        self._player_strength = player_input.strength

    @property
    def _move_selector_args(self):
        return {
            RandomMoveSelector: {},
            MinimaxMoveSelectorPy: {
                # "evaluator": DEFAULT_MINIMAX_EVALUATOR,
                "search_depth": self._player_strength
            }
        }

    def _build_human_player(self):
        return self._player_constructor()

    def _build_ai_player(self):
        if self._move_selector_constructor is None:
            self._move_selector_constructor = MinimaxMoveSelectorPy

        if (self._move_selector_constructor == MinimaxMoveSelectorPy and
                self._player_strength is None):
            self._player_strength = self._default_player_strengths[self._color]
        constructor_kwargs = self._move_selector_args[self._move_selector_constructor]
        move_selector = self._move_selector_constructor(**constructor_kwargs)

        return AIPlayer(color=self._color, move_selector=move_selector)

    @property
    def _player_dispatch(self):
        return {
            HumanPlayer: self._build_human_player,
            AIPlayer: self._build_ai_player
        }

    def build(self):
        return self._player_dispatch[self._player_constructor]()


class RedAndBlackPlayersBuilder:

    def __init__(self, xiangqi_command: XiangqiGameCommand):
        self._command = xiangqi_command

    def build(self):
        red_player = SinglePlayerBuilder(
            color=PieceColor.kRed,
            player_input=self._command.red_player_input).build()

        black_player = SinglePlayerBuilder(
            color=PieceColor.kBlk,
            player_input=self._command.black_player_input).build()

        return {PieceColor.kRed: red_player, PieceColor.kBlk: black_player}


def main():
    command_line = XiangqiGameCommandLine()
    interpreted_command = command_line.get_args(["-ba", "random"])
    players = RedAndBlackPlayersBuilder(interpreted_command).build()
    print(players[PieceColor.kRed])


if __name__ == "__main__":
    main()
