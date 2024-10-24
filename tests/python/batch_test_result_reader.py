import batch_testing as bt
import xiangqi_bindings as bindings


if __name__ == "__main__":

    full_batch_summary_032bit = bt.FullBatchSummary(
        batch_id="20241023153411705876"
    )
    full_batch_summary_064bit = bt.FullBatchSummary(
        batch_id="20241023153611738579"
    )
    full_batch_summary_128bit = bt.FullBatchSummary(
        batch_id="20241023153811028455"
    )

    batch_ids = [
        "20241023153411705876",
        "20241023153611738579",
        "20241023153811028455",
    ]

    multi_batch_summarizer = bt.MultiBatchDataSummarizer(batch_ids=batch_ids)

    full_batch_summary_032bit.plot_all_collision_data(
        color=bindings.PieceColor.kRed,
        by_game_id=False
    )
    full_batch_summary_032bit.plot_all_collision_data(
        color=bindings.PieceColor.kBlk,
        by_game_id=False
    )
    full_batch_summary_064bit.plot_all_collision_data(
        color=bindings.PieceColor.kRed,
        by_game_id=False
    )
    full_batch_summary_064bit.plot_all_collision_data(
        color=bindings.PieceColor.kBlk,
        by_game_id=False
    )
    full_batch_summary_128bit.plot_all_collision_data(
        color=bindings.PieceColor.kRed,
        by_game_id=False
    )
    full_batch_summary_128bit.plot_all_collision_data(
        color=bindings.PieceColor.kBlk,
        by_game_id=False
    )

    print("pause")
