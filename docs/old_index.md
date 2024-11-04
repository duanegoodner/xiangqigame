
# Introduction

This project consists of a C++ Artificial Intelligence (AI) engine for the board game [Xiangqi](https://en.wikipedia.org/wiki/Xiangqi) (also known as Chinese chess).

The AI utilizes the Minimax algorithm enhanced by Alpha-Beta pruning to select moves. A points value for each encountered game state is calculated based of the type and position of each piece present on the board. These pionts values are stored in a Zobrist hashing-based transposition table to prevent repeating calculations of a previously seen board state.

The C++ core of the engine is integrated with an outer Python layer using Pybind11, facilitating installation as a standard Python package via [pip](https://pip.pypa.io/en/stable/). The Python layer provides a command-line interface (CLI) that supports multiple game modes: AI vs. AI, AI vs. Human, and Human vs. Human. Additionally, the CLI offers configurable settings for the Minimax search depth and Zobrist key size. It also allows users to access detailed data about the internal workings of the search algorithm generated during games.

