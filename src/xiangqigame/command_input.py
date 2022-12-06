import argparse
from dataclasses import dataclass


@dataclass
class CommandLineDefaults:
    red_player_type: str = "ai"
    red_algo: str = "minimax"
    red_strength: str = "2"
    black_player_type: str = "ai"


class XiangqiGameCommandLine:

    _default_args = {
            "red_player_type": "ai",
            "red_algo": "minimax",
            "red_strength": "1",
            "black_player_type": "ai",
            "black_algo": "minimax",
            "black_strength": "2",
        }

    def __init__(self):
        self._parser = argparse.ArgumentParser()
        self._defaults = CommandLineDefaults()

    def _attach_args(self):
        self._parser.add_argument(
            "-rt",
            "--red_player_type",
            choices=["person", "ai"],
            required=False,
        )

        self._parser.add_argument(
            "-ra",
            "--red_algo",
            choices=["random", "minimax"],
            required=False
        )

        self._parser.add_argument(
            "-rst",
            "--red_strength",
            type=int,
            choices=range(4),
            required=False
        )

        self._parser.add_argument(
            "-bt",
            "--black_player_type",
            choices=["person", "ai"],
            required=False
        )

        self._parser.add_argument(
            "-ba",
            "--black_algo",
            choices=["random", "minimax"],
            required=False
        )

        self._parser.add_argument(
            "-bst",
            "--black_strength",
            type=int,
            choices=range(4),
            required=False
        )

    def get_args(self, *args):
        self._attach_args()
        args_namespace = self._parser.parse_args(*args)
        for key, value in args_namespace.__dict__.items():
            if not value:
                args_namespace.__dict__[key] = self._default_args[key]
        args_namespace.red_strength = int(args_namespace.red_strength)
        args_namespace.black_strength = int(args_namespace.black_strength)
        return args_namespace


def main():
    command_retriever = XiangqiGameCommandLine()
    my_args = command_retriever.get_args()
    print(my_args)


if __name__ == "__main__":
    main()
