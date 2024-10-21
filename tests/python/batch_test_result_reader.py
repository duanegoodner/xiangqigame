import batch_testing as bt


if __name__ == "__main__":

    full_batch_summary_32bit = bt.FullBatchSummary(
        batch_id="20241020180134341665"
    )
    full_batch_summary_64bit = bt.FullBatchSummary(
        batch_id="20241020183511307341"
    )
    full_batch_summary_128bit = bt.FullBatchSummary(
        batch_id="20241020190909114459"
    )

    batch_ids = [
        "20241020180134341665",
        "20241020183511307341",
        "20241020190909114459",
    ]

    multi_batch_summarizer = bt.MultiBatchDataSummarizer(batch_ids=batch_ids)

    print("pause")
