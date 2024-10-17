import batch_testing as bt


if __name__ == "__main__":

    batch_ids = [
        "20241016221027549952",
        "20241016221233448523",
    ]

    multi_batch_summarizer = bt.MultiBatchDataSummarizer(batch_ids=batch_ids)

    print("pause")
