import json
from game.game import Game
from common.enums import PieceColor


def run():
    with open('data/game_start.json') as game_info:
        game_config = json.load(game_info)
    my_game = Game(game_config)
    # print(my_game._board)
    # print(my_game._board.calc_final_moves_of(PieceColor.RED))
    # print(my_game._board)
    # print(my_game.is_in_check(PieceColor.BLACK))
    # print(my_game._board.get_all_spaces_occupied_by(PieceColor.RED))
    my_game.play()


if __name__ == '__main__':
    run()
