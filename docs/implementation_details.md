
# Design and Implementation Highlights

This document provides a high level overview of the project  structure, and the responsibilities of various components and modules. Some finer-grained details of the most critical modules and classes are also included.

## Top Level Components

Xiangqigame has three main components: a C++ core to enable fast computatations, a Python outer layer, and a Pybind11 module that provides an interface between the C++ and Python layers.

Each of these components resides in a different sub-directory of `./src`.

```
./src
├── bindings
├── core
├── data
└── xiangqigame
```
Note that ./src also has a `data/` sub-directory containing .json files used for points calculations.

## C++ Core Engine

### Libraries

Below is the high-level file structure of `./src/core`.

```
./src/core
├── board_components
├── board_utilities
├── common
├── game_board
├── hash_calculator
├── json_interface
├── json_internal
├── json_io
├── json_validation
├── move_calculator
├── move_evaluators
├── piece_moves
├── piece_points
├── piece_points_bpo
├── points_containers
└── utility_functs
```

- **board_components**, **board_utilities**, and **common** contain header-only libraries with structs, stand-alone functions, constants, and tyepdefs that are used by multiple other libraries.

- **game_board** class `NewGameBoard` which complies with CRTP interface `SpaceInfoProvider` defined in `move_evaluators`. 

- **hash_calculator**


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


```
.
├── app.py
├── command_input.py
├── core_dataclass_mirrors.py
├── enums.py
├── game_analysis.py
├── game_interfaces.py
├── game_output_generator.py
├── game.py
├── game_summary_io.py
├── game_summary_plot_manager.py
├── game_summary_plotters.py
├── game_summary.py
├── handlers
│   ├── errors.py
│   ├── __init__.py
│   └── signals.py
├── __init__.py
├── __main__.py
├── move_translator.py
├── output_path_builder.py
├── piece_info_extractor.py
├── piece_points.py
├── player_builder.py
├── players.py
├── player_summary.py
├── terminal_output.py
└── terminal_output.py

```

### Players

### Game

### Packaging

### UI


<div id="svg-container" style="width: 740px; height: 400px; overflow: scroll; border: 1px solid black;">
  <img id="zoomable-svg" src="path_to_your_svg.svg" alt="Your SVG Image" style="width: 100%; height: auto;">
</div>

<script>
  var container = document.getElementById('svg-container');
  var img = document.getElementById('zoomable-svg');

  // Initial scale
  var scale = 1;

  // Function to handle zooming
  function zoomIn() {
    scale *= 1.1;  // Increase the scale by 10%
    img.style.transform = 'scale(' + scale + ')';
  }

  function zoomOut() {
    scale /= 1.1;  // Decrease the scale by 10%
    img.style.transform = 'scale(' + scale + ')';
  }

  // Adding keyboard events for zoom in and zoom out
  container.addEventListener('keydown', function(e) {
    if (e.key === '+') {
      zoomIn();
    } else if (e.key === '-') {
      zoomOut();
    }
  });

  // Make the container focusable
  container.setAttribute('tabindex', '0');
  container.style.outline = 'none';
</script>



