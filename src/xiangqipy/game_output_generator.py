"""
@file game_output_generator.py

Contains the GameOutputGenerator class.
"""

from pathlib import Path

from xiangqipy.game_summary import GameSummary
from xiangqipy.game_summary_io import export_game_summary
from xiangqipy.game_summary_plot_manager import GameSummaryPlotManager


class GameOutputGenerator:
    """
    Outputs GameSummary to .json file, and saves plots to a .png file.
    """

    def __init__(
        self,
        game_summary: GameSummary,
        output_dir_suffix: str = None,
        custom_output_root: Path = None,
        # game_collection_id: str = None,
    ):
        self.game_summary = game_summary
        self.output_dir_suffix = output_dir_suffix
        self.custom_output_root = custom_output_root
        self.output_dir = self.create_output_dir()
        # self.game_collection_id = game_collection_id

    def create_output_dir(self) -> Path:

        output_dir_name = self.game_summary.game_id
        if self.output_dir_suffix:
            output_dir_name += f"-{self.output_dir_suffix}"

        if self.custom_output_root:
            output_root = self.custom_output_root
        else:
            output_root = (
                Path(__file__).parent.parent.parent / "data" / "game_summaries"
            )

        game_output_dir_name = self.game_summary.game_id
        if self.output_dir_suffix:
            game_output_dir_name += f"-{self.output_dir_suffix}"

        game_output_dir = output_root / game_output_dir_name
        game_output_dir.mkdir(parents=True, exist_ok=True)

        return game_output_dir

    def generate_output(self):
        # output_dir = self.create_output_dir()
        plot_manager = GameSummaryPlotManager(game_summary=self.game_summary)
        plot_manager.plot(show_plot=False)

        game_summary_path = self.output_dir / f"{self.game_summary.game_id}.json"
        export_game_summary(
            game_summary=self.game_summary, path=game_summary_path
        )

        plot_manager.save_figure(
            path=self.output_dir / f"{self.game_summary.game_id}.png"
        )
