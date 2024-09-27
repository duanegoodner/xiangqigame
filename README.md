
# xiangqigame 
C++ AI engine for [Xiangqi](https://en.wikipedia.org/wiki/Xiangqi) (a.k.a. Chinese chess) wrapped in a Python package for playing AI vs. AI, AI vs. Human, and Human vs. Human games.

## Getting Started

### Installing and running natively

#### Requirements

- A C++ compiler that supports the C++ 20 standard
- cmake 3.21+
- Python 3.8+

#### Recommended
- Anaconda or Miniconda

```
git clone https://github.com/duanegoodner/xiangqigame
cd xiangqigame
conda create --name xiangqigame python=3.12 -y
conda activate xiangqigame
pip install .
play_xiangqi
```

#### Running  in a containerized environment

- Docker 18.06+
- Docker Compose 1.25.0+

```
git clone https://github.com/duanegoodner/xiangqigame
cd xiangqigame/.devcontainer
docker compose build
docker compose up -d
docker exec -it cpyenv /bin/zsh

```


## Description

* Can be played by human and/or AI player
* Game management and user interface implemented in Python
* Piece move calcluations and minimax search algorithm (used by AI player) implemented in C++ and exposed to Python using [pybind11](https://github.com/pybind/pybind11)

<!-- ![demo](doc/xiangqigame_demo.gif) -->

## Brief Project History
* Began as an assignment for Oregon State undergraduae course CS 162 in 2020 (at that time, it was just an implementation of piece moves in Python)
* As my programming skills have improved over time, I have used this project as a benchmark, occasionally re-implementing / enhancing it.
