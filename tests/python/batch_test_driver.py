from dataclasses import dataclass
from typing import Any, Dict

import batch_testing as bt


DEFAULT_NUM_GAMES = 10


@dataclass
class BatchTestConditions:
    run_kwargs: Dict[str, Any]
    num_games: int = DEFAULT_NUM_GAMES


test_conditions_01 = BatchTestConditions(
    run_kwargs={
        "red_strength": 3,
        "red_key_size": 64,
        "black_strength": 3,
        "black_key_size": 64,
        "save_summary": True,
        "output_dir_suffix": "R-d3-k064-B-d3-k064",
    },
)

test_conditions_02 = BatchTestConditions(
    run_kwargs={
        "red_strength": 3,
        "red_key_size": 128,
        "black_strength": 3,
        "black_key_size": 64,
        "save_summary": True,
        "output_dir_suffix": "R-d3-k128-B-d3-k064",
    }
)

test_conditions_03 = BatchTestConditions(
    run_kwargs={
        "red_strength": 3,
        "red_key_size": 64,
        "black_strength": 3,
        "black_key_size": 128,
        "save_summary": True,
        "output_dir_suffix": "R-d3-k064-B-d3-k128",
    }
)

test_conditions_04 = BatchTestConditions(
    run_kwargs={
        "red_strength": 4,
        "red_key_size": 64,
        "black_strength": 4,
        "black_key_size": 64,
        "save_summary": True,
        "output_dir_suffix": "R-d4-k064-B-d4-k064",
    },
)

test_conditions_05 = BatchTestConditions(
    run_kwargs={
        "red_strength": 4,
        "red_key_size": 128,
        "black_strength": 4,
        "black_key_size": 64,
        "save_summary": True,
        "output_dir_suffix": "R-d4-k128-B-d4-k064",
    },
)

test_conditions_06 = BatchTestConditions(
    run_kwargs={
        "red_strength": 4,
        "red_key_size": 64,
        "black_strength": 4,
        "black_key_size": 128,
        "save_summary": True,
        "output_dir_suffix": "R-d4-k064-B-d4-k128",
    },
)

test_conditions_07 = BatchTestConditions(
    run_kwargs={
        "red_strength": 5,
        "red_key_size": 64,
        "black_strength": 5,
        "black_key_size": 64,
        "save_summary": True,
        "output_dir_suffix": "R-d5-k064-B-d5-k064",
    },
)

test_conditions_08 = BatchTestConditions(
    run_kwargs={
        "red_strength": 5,
        "red_key_size": 128,
        "black_strength": 5,
        "black_key_size": 64,
        "save_summary": True,
        "output_dir_suffix": "R-d5-k128-B-d5-k064",
    },
)

test_conditions_09 = BatchTestConditions(
    run_kwargs={
        "red_strength": 5,
        "red_key_size": 64,
        "black_strength": 5,
        "black_key_size": 128,
        "save_summary": True,
        "output_dir_suffix": "R-d5-k064-B-d5-k128",
    },
)




def main():
    test_conditions = [
        test_conditions_01,
        test_conditions_02,
        test_conditions_03,
        test_conditions_04,
        test_conditions_05,
        test_conditions_06,
        test_conditions_07,
        test_conditions_08,
        test_conditions_09,
    ]

    for test_condition in test_conditions:
        batch_tester = bt.BatchTester(
            app_run_kwargs=test_condition.run_kwargs,
            num_games=test_condition.num_games,
        )
        batch_tester.run_all_tests()


if __name__ == "__main__":
    main()
