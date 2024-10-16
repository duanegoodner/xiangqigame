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
        game_collection_id: str = None,
    ):
        self.game_summary = game_summary
        self.output_dir_suffix = output_dir_suffix
        self.game_collection_id = game_collection_id

    def create_output_dir(self) -> Path:
        output_dir_name = self.game_summary.game_id
        if self.output_dir_suffix:
            output_dir_name += f"-{self.output_dir_suffix}"

        output_root = (
            Path(__file__).parent.parent.parent / "data" / "game_summaries"
        )

        if self.game_collection_id:
            collection_dir_name = self.game_collection_id
            if self.output_dir_suffix:
                collection_dir_name += f"-{self.output_dir_suffix}"
            output_root /= collection_dir_name

        game_output_dir_name = self.game_summary.game_id
        if self.output_dir_suffix:
            game_output_dir_name += f"-{self.output_dir_suffix}"

        game_output_dir = output_root / game_output_dir_name
        game_output_dir.mkdir(parents=True, exist_ok=True)

        return game_output_dir



        # output_dir = (
        #     Path(__file__).parent.parent.parent
        #     / "data"
        #     / "game_summaries"
        #     / output_dir_name
        # )
        # output_dir_str = f"./data/game_summaries/{self.game_summary.game_id}"
        # if self.output_dir_suffix:
        #     output_dir_str += f"-{self.output_dir_suffix}"
        # output_dir = Path(output_dir_str)
        # output_dir.mkdir(parents=True, exist_ok=True)
        #
        # return output_dir

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
