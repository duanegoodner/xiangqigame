import batch_testing as bt
import xiangqi_bindings as bindings


if __name__ == "__main__":

    full_batch_summary_32bit = bt.FullBatchSummary(
        batch_id="20241021205012305613"
    )
    full_batch_summary_64bit = bt.FullBatchSummary(
        batch_id="20241021205130512080"
    )
    full_batch_summary_128bit = bt.FullBatchSummary(
        batch_id="20241021205248124864"
    )

    batch_ids = [
        "20241021205012305613",
        "20241021205130512080",
        "20241021205248124864",
    ]

    multi_batch_summarizer = bt.MultiBatchDataSummarizer(batch_ids=batch_ids)

    red_32bit_collision_data = (
        full_batch_summary_32bit.get_player_collision_data(
            color=bindings.PieceColor.kRed
        )
    )

    print("pause")
