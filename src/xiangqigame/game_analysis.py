from xiangqigame_core import PieceColor

import xiangqigame.app as app
import xiangqigame.game_summary_plotter as gsp
import xiangqigame.search_results_by_type_plotter as stp
import matplotlib.pyplot as plt
import xiangqigame_core as core


if __name__ == "__main__":

    game_summary = app.run(red_strength=3, black_strength=3)

    test_val = game_summary.get_player_summary(player=core.PieceColor.kRed).first_searches_by_type_and_depth

    results_by_type_plotter = stp.SearchResultByTypePlotter(
        game_summary=game_summary
    )
    results_by_type_plotter.plot()
    #
    # game_plotter = gsp.GameSummaryPlotter(game_summary=game_summary)
    #
    # fig, ax = plt.subplots()
    # game_plotter.plot_eval_scores(ax)
    # plt.show()


    # results_by_depth_plotter = gsp.SearchResultsByDepthPlotter(
    #     game_summary=game_summary
    # )
    # results_by_depth_plotter.plot_multiple_search_result_types_by_depth(
    #     player=core.PieceColor.kRed,
    #     result_types=[
    #         core.MinimaxResultType.TrTableHitStandard,
    #         core.MinimaxResultType.TrTableHitEvaluatorLoses,
    #         core.MinimaxResultType.TrTableHitEvaluatorWins,
    #         core.MinimaxResultType.EvaluatorLoses,
    #         core.MinimaxResultType.EvaluatorWins,
    #         core.MinimaxResultType.FullyEvaluatedNode,
    #         core.MinimaxResultType.StandardLeaf,
    #         core.MinimaxResultType.AlphaPrune,
    #         core.MinimaxResultType.BetaPrune,
    #     ],
    # )


    #
    # game_summary_plotter = gsp.GameSummaryPlotter(game_summary=game_summary)
    # fig, ax = plt.subplots()
    # game_summary_plotter.plot_first_search_by_type(player=core.PieceColor.kRed, ax=ax)
    # plt.show()


    # plt.show()
    print("pause")

    # game_plotter = gsp.GameSummaryPlotter(game_summary=game_summary)
    #
    # game_plotter.plot_multiple_search_result_types_by_depth(
    #     player=core.PieceColor.kBlk,
    #     result_types=[
    #         core.MinimaxResultType.TrTableHitStandard,
    #         core.MinimaxResultType.TrTableHitEvaluatorLoses,
    #         core.MinimaxResultType.TrTableHitEvaluatorWins,
    #         core.MinimaxResultType.EvaluatorLoses,
    #         core.MinimaxResultType.EvaluatorWins,
    #         core.MinimaxResultType.FullyEvaluatedNode,
    #         core.MinimaxResultType.StandardLeaf,
    #         core.MinimaxResultType.AlphaPrune,
    #         core.MinimaxResultType.BetaPrune,
    #     ],
    # )

    # fig, ax = plt.subplots()
    # game_plotter.plot_search_result_type_by_depth(
    #     player=core.PieceColor.kBlk,
    #     result_type=core.MinimaxResultType.FullyEvaluatedNode,
    #     ax=ax
    # )


    # game_plotter.plot_search_results_by_depth(
    #     player=core.PieceColor.kBlk,
    #     result_type=core.MinimaxResultType.AlphaPrune,
    # )

