from multi_games_run import BatchTester


def main():
    run_kwargs_01={
        "red_strength": 3,
        "red_key_size": 64,
        "black_strength": 3,
        "black_key_size": 64,
        "save_summary": True,
        "output_dir_suffix": "R-d3-k064-B-d3-k064",
    }
    num_games_01=10

    run_kwargs_02={
        "red_strength": 3,
        "red_key_size": 128,
        "black_strength": 3,
        "black_key_size": 64,
        "save_summary": True,
        "output_dir_suffix": "R-d3-k128-B-d3-k064",
    }
    num_games_02=10

    run_kwargs_03={
        "red_strength": 3,
        "red_key_size": 64,
        "black_strength": 3,
        "black_key_size": 128,
        "save_summary": True,
        "output_dir_suffix": "R-d3-k064-B-d3-k128",
    }
    num_games_03=10

    tester_01 = BatchTester(
        app_run_kwargs=run_kwargs_01,
        num_games=num_games_01,
    )
    tester_01.run_all_tests()


if __name__ == "__main__":
    main()



