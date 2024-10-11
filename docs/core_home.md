\mainpage


# Overview

This project consists of a C++ Artificial Intelligence (AI) engine for the board game [Xiangqi](https://en.wikipedia.org/wiki/Xiangqi) (also known as Chinese chess) and support playing games in AI vs. AI, AI vs. Human, and Human vs. Human modes.


## Core Algorithm

The AI utilizes the Minimax algorithm enhanced by Alpha-Beta pruning to select moves. A transposition table based on Zobrist hashing is implemented to prevent re-calculation of previously seen board states.


## Architecture





## Integration with Python

The C++ core is integrated with an outer Python layer using Pybind11. The Python layer:
    - Facilitates installation as a standard Python package via [pip](https://pip.pypa.io/en/stable/)
    - Provides a command-line interface (CLI) for running a game and configuring settings such as the Minimax search depth and Zobrist key size.
    - Displays a text-based game board
    Additionally, the CLI offers configurable settings for 
    - Allows users to access detailed data about the internal workings of the search algorithm generated during games.


## Build









[Click here to view the SVG!](../../plantuml/xiangqigame_core.svg)

