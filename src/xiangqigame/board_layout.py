# from xiangqigame.piece_definitions import PColor
import cpp_modules.game_board_py.GameBoardPy as gbp


class BoardLayout:
    num_ranks = 10
    num_files = 9
    river_edges = {gbp.PieceColor.kBlk: 4, gbp.PieceColor.kRed: 5}
    castle_edges = {
        # (min_rank, max_rank, min_file, max_file)
        gbp.PieceColor.kBlk: (0, 2, 3, 5),
        gbp.PieceColor.kRed: (7, 9, 3, 5)
    }

    castle_slices = {
        gbp.PieceColor.kBlk: (slice(0, 3), slice(3, 6)),
        gbp.PieceColor.kRed: (slice(7, num_ranks), slice(3, 6))}

    castle_coords = {
        gbp.PieceColor.kBlk: tuple((rank, file) for rank in range(0, 3)
                          for file in range(3, 6)),
        gbp.PieceColor.kRed: tuple((rank, file) for rank in range(7, num_ranks)
                          for file in range(3, 6))
    }

    opponent_of = {gbp.PieceColor.kBlk: gbp.PieceColor.kRed, gbp.PieceColor.kRed: gbp.PieceColor.kBlk}
