
# Design and Implementation Details

Xiangqigame has three main components: a C++ core to enable fast computatations, a Python outer layer, and a Pybind11 module that provides an interface between the C++ and Python layers.

## C++ Core Engine

### Internal Game Board

### Minimax Algorithm

#### Piece Points Values

#### Board Configuration Scores

#### Alpha-Beta Pruning

#### Pre-search sorting

#### Zobrist Hasing

#### Transposition Tables

#### 64 bit vs 128 bit hash keys

### CRTP Class Interfaces


## Pybind11 Module

### MinimaxMoveEvaluator
### RandomMoveEvaluator
### SearchSummary
### BoardSpace
### GamePiece
### Move
### MoveCollection
### BestMoves
### ExecutedMove
### PieceColor
### PieceType
### size_of_points_type
### is_signed_points_type
### MinimaxResultType
### GameBoard


## Python Package Responsibilities

### Players

### Game

### Packaging

### UI


