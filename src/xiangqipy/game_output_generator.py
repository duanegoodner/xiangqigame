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
    def __init__(self, game_summary: GameSummary, output_dir_suffix: str = ""):
        self.game_summary = game_summary
        self.output_dir_suffix = output_dir_suffix

    def create_output_dir(self) -> Path:
        output_dir_str = f"./data/game_summaries/{self.game_summary.game_id}"
        if self.output_dir_suffix:
            output_dir_str += f"-{self.output_dir_suffix}"
        output_dir = Path(output_dir_str)
        output_dir.mkdir(parents=True, exist_ok=True)

        return output_dir

    def generate_output(self):
        output_dir = self.create_output_dir()
        plot_manager = GameSummaryPlotManager(game_summary=self.game_summary)
        plot_manager.plot(show_plot=False)

        game_summary_path = output_dir / f"{self.game_summary.game_id}.json"
        export_game_summary(
            game_summary=self.game_summary, path=game_summary_path
        )

        plot_manager.save_figure(
            path=output_dir / f"{self.game_summary.game_id}.png"
        )
