import batch_testing as bt
import xiangqi_bindings as bindings


if __name__ == "__main__":

    full_batch_summary_032bit = bt.FullBatchSummary(
        batch_id="20241022154838753207"
    )
    full_batch_summary_064bit = bt.FullBatchSummary(
        batch_id="20241022155037560595"
    )
    # full_batch_summary_128bit = bt.FullBatchSummary(
    #     batch_id="20241022154409603175"
    # )

    batch_ids = [
        "20241022154838753207",
        "20241022155037560595",
        # "20241022154409603175",
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
    # full_batch_summary_128bit.plot_all_collision_data(
    #     color=bindings.PieceColor.kRed,
    #     by_game_id=False
    # )
    # full_batch_summary_128bit.plot_all_collision_data(
    #     color=bindings.PieceColor.kBlk,
    #     by_game_id=False
    # )

    print("pause")
