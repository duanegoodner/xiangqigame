from xiangqigame.command_input import (
    XiangqiGameCommand,
    XiangqiGameCommandLine,
    PlayerInput,
)

from xiangqigame_core import (
    GameBoard,
    MinimaxMoveEvaluator,
    PieceColor,
    RandomMoveEvaluator,
)
from xiangqigame.players import AIPlayer, HumanPlayer


class SinglePlayerBuilder:

    _default_player_strengths = {PieceColor.kRed: 2, PieceColor.kBlk: 3}

    def __init__(
        self, player_input: PlayerInput, color: PieceColor, game_board: GameBoard
    ):

        self._color = color
        if player_input.player_type is None:
            player_input.player_type = AIPlayer
        self._player_constructor = player_input.player_type
        self._move_evaluator_constructor = player_input.algo
        self._player_strength = player_input.strength
        self._game_board = game_board

    @property
    def _move_evaluator_args(self):
        return {
            RandomMoveEvaluator: {
                "evaluating_player": self._color,
                "game_board": self._game_board,
            },
            MinimaxMoveEvaluator: {
                "evaluating_player": self._color,
                "starting_search_depth": self._player_strength,
                "game_board": self._game_board,
            },
        }

    def _build_human_player(self):
        return self._player_constructor()

    def _build_ai_player(self):
        if self._move_evaluator_constructor is None:
            self._move_evaluator_constructor = MinimaxMoveEvaluator

        if (
            self._move_evaluator_constructor == MinimaxMoveEvaluator
            and self._player_strength is None
        ):
            self._player_strength = self._default_player_strengths[self._color]
        constructor_kwargs = self._move_evaluator_args[self._move_evaluator_constructor]
        move_evaluator = self._move_evaluator_constructor(**constructor_kwargs)

        return AIPlayer(color=self._color, move_evaluator=move_evaluator)

    @property
    def _player_dispatch(self):
        return {HumanPlayer: self._build_human_player, AIPlayer: self._build_ai_player}

    def build(self):
        return self._player_dispatch[self._player_constructor]()


class RedAndBlackPlayersBuilder:

    def __init__(self, xiangqi_command: XiangqiGameCommand, game_board: GameBoard):
        self._command = xiangqi_command
        self._board = game_board

    def build(self):
        red_player = SinglePlayerBuilder(
            color=PieceColor.kRed,
            player_input=self._command.red_player_input,
            game_board=self._board,
        ).build()

        black_player = SinglePlayerBuilder(
            color=PieceColor.kBlk,
            player_input=self._command.black_player_input,
            game_board=self._board,
        ).build()

        return {PieceColor.kRed: red_player, PieceColor.kBlk: black_player}


def main():
    command_line = XiangqiGameCommandLine()
    interpreted_command = command_line.get_args(["-ba", "random"])
    players = RedAndBlackPlayersBuilder(interpreted_command).build()
    print(players[PieceColor.kRed])


if __name__ == "__main__":
    main()
