from xiangqigame_core import PieceColor

import xiangqigame.app as app
import game_summary_plotter as gsp
import matplotlib.pyplot as plt
import xiangqigame_core as core


if __name__ == "__main__":

    game_summary = app.run(red_strength=3, black_strength=3)
    game_plotter = gsp.GameSummaryPlotter(game_summary=game_summary)

    fig, ax = plt.subplots()

    game_plotter.plot_eval_scores(ax)
    # game_plotter.plot_pruning_counts(ax)
    # game_plotter.plot_end_game_leaves(player=core.PieceColor.kBlk, ax=ax)
    item = game_summary.player_summaries.black.search_summaries.first_searches_by_type_and_depth
    plt.show()

