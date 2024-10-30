import xiangqipy.app as app


if __name__ == "__main__":

    game_summary = app.run(
        red_strength=3,
        red_number_zobrist_states=2,
        black_strength=3,
        black_number_zobrist_states=2,
    )

    print("pause")
