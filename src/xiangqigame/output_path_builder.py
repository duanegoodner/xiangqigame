from xiangqigame.game_summary import GameSummary
from pathlib import Path


def create_output_path(
    game_summary: GameSummary, file_ext: str, output_dir_suffix: str = ""
):
    output_dir_str = f"./data/game_summaries/{game_summary.game_id}"
    if output_dir_suffix:
        output_dir_str += f"-{output_dir_suffix}"

    if not file_ext.startswith("."):
        file_ext = "." + file_ext
    output_path = Path(output_dir_str) / f"{game_summary.game_id}{file_ext}"
    if output_path.exists():
        raise FileExistsError(f"Output path already exists: {output_path}")
    output_path.parent.mkdir(parents=True, exist_ok=True)
    return output_path
