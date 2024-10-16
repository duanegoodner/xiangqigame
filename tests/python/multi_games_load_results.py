from pathlib import Path

import pandas as pd


def load_multi_games_results(games_collection_id: str) -> pd.DataFrame:
    output_root = (
        Path(__file__).parent.parent.parent / "data" / "multi_game_summaries"
    )
    result_dir = None
    for path in output_root.iterdir():
        if path.name.startswith(games_collection_id):
            result_dir = path
    if result_dir is None:
        raise FileNotFoundError(
            f"Directory with {games_collection_id} in name not found."
        )

    result_df = pd.read_feather(
        Path(result_dir / f"{games_collection_id}.feather")
    )
    return result_df


if __name__ == "__main__":

    df = load_multi_games_results(games_collection_id="20241015135829066307")
    md_df = df[
        [
            "move_counts",
            "game_state",
            "kRed_nodes_per_move_mean",
            "kRed_time_per_move_mean_s",
            "kRed_known_hash_collisions",
            "kBlk_nodes_per_move_mean",
            "kBlk_time_per_move_mean_s",
            "kBlk_known_hash_collisions",
        ]
    ].to_markdown()
    print(md_df)
