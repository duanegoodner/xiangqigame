import batch_testing as bt


if __name__ == '__main__':

    batch_ids = [
        "20241016152507993956",
        "20241016152530959562",
        "20241016152556479095"
    ]

    multi_batch_summarizer = bt.MultiBatchDataSummarizer(batch_ids=batch_ids)

    print("pause")