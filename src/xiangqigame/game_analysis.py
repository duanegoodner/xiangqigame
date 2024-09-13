import xiangqigame.app as app
import xiangqigame_core as core


if __name__ == "__main__":

    minimax_result_type_info = core.MinimaxResultType.__dict__["__entries"]

    minimax_result_type_enum_decoder = {
        val[0]: key for key, val in minimax_result_type_info.items()
    }
    minimax_result_type_int_decoder = {
        int(val[0]): key for key, val in minimax_result_type_info.items()
    }

    # minimax_result_type_decoder = {
    #     member: member.name for member in core.MinimaxResultType
    # }

    game_summary = app.run(red_strength=3, black_strength=3)
    print(game_summary)
