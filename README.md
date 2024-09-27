
# xiangqigame 
C++ AI engine for [Xiangqi](https://en.wikipedia.org/wiki/Xiangqi) (a.k.a. Chinese chess) wrapped in a Python package for playing AI vs. AI, AI vs. Human, and Human vs. Human games.



## Description

* Can be played by human and/or AI player
* Game management and user interface implemented in Python
* Piece move calcluations and minimax search algorithm (used by AI player) implemented in C++ and exposed to Python using [pybind11](https://github.com/pybind/pybind11)

<!-- ![demo](doc/xiangqigame_demo.gif) -->

## Brief Project History
* Began as an assignment for Oregon State undergraduae course CS 162 in 2020 (at that time, it was just an implementation of piece moves in Python)
* As my programming skills have improved over time, I have used this project as a benchmark, occasionally re-implementing / enhancing it.
