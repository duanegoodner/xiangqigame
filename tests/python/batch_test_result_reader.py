import batch_testing as bt


if __name__ == "__main__":

    batch_ids = [
        "20241017082055105920",
        "20241017082150589202",
    ]

    multi_batch_summarizer = bt.MultiBatchDataSummarizer(batch_ids=batch_ids)

    print("pause")
