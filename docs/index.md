


# Introduction

This project consists of a C++ Artificial Intelligence (AI) engine for the board game [Xiangqi](https://en.wikipedia.org/wiki/Xiangqi) (also known as Chinese chess). The AI utilizes the Minimax algorithm enhanced by Alpha-Beta pruning to select moves. A points value for each encountered game state is calculated based of the type and position of each piece present on the board. These pionts values are stored in a Zobrist hashing-based transposition table to prevent repeating calculations of a previously seen board state.

The C++ core of the engine is integrated with an outer Python layer using Pybind11, facilitating installation as a standard Python package via [pip](https://pip.pypa.io/en/stable/). The Python layer provides a command-line interface (CLI) that supports multiple game modes: AI vs. AI, AI vs. Human, and Human vs. Human. Additionally, the CLI offers configurable settings for the Minimax search depth and Zobrist key size. It also allows users to access detailed data about the internal workings of the search algorithm generated during games.


## Computational complexity of Xiangqi vs Chess
Similar to western Chess, Xiangqi is a two-player strategy game played on a grid, and each player begins with an identical 16-piece set of multiple types of pieces, differentiated only by color. Each game's rules define the permissible moves for each piece type. In both games, players alternate turns, making one legal move per turn. If an opponent's piece occupies the destination of a legally executed move, that piece is captured and removed from the game. The objective of both games is to capture the opponent's primary piece.

The Xiangqi game board is larger than the board in Chess (9 x 10 vs. 8 x 8), but the Xiangqi rules place more restrictions on the allowed move destination of some piece types. The average number of legal moves available from any given board position, known as the "branching factor", is approximately 38 for Xiangqi and 35 for Chess. The number of legal board states is often estimated to be ~10<sup>43</sup>
