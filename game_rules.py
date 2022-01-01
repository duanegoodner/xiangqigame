from enums import PieceColor


board_dim = {
    'num_files': 9,
    'num_ranks': 10
}


opponent_of = {
    PieceColor.RED: PieceColor.BLACK,
    PieceColor.BLACK: PieceColor.RED
}

forward_direction = {
    PieceColor.RED: 1,
    PieceColor.BLACK: -1
}

castles = {
    PieceColor.RED: [
        (row, col) for row in range(3) for col in range(4, 7)
    ],
    PieceColor.BLACK: [
        (row, col) for row in range(7, 10) for col in range(4, 7)
    ]
}

river_edges = {
    PieceColor.RED: 4,
    PieceColor.BLACK: 5
}