from docutils.nodes import abbreviation

import batch_testing as bt


if __name__ == "__main__":
    test_abbreviations = [
        "2-R-d3-k064-B-d3-k064",
        "2-R-d3-k064-B-d3-k064",
    ]
    test_conditions = [
        bt.BatchTestConditions.from_abbreviation(
            abbreviation=abbreviation,
        )
        for abbreviation in test_abbreviations
    ]

    for condition in test_conditions:
        batch_tester = bt.BatchTester(
            app_run_kwargs=condition.run_kwargs, num_games=condition.num_games
        )
        batch_tester.run_all_tests()
