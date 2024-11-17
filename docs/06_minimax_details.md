# Minimax Details

The heavy-lifting in our AI engine is handled by the moveselection::MinimaxMoveEvaluator class, an implementation of the MoveEvaluator interface. The interface requires implementation of a single method: ImplementMoveSelection which takes a gameboard::MoveCollection of available moves as its only argument, and returns a single gameboard::Move selected from the available moves. 

Inside moveselection::MinimaxMoveEvaluator.SelectMove, we do the following:

## Points Assignment


## Pre-Search Sorting of Available Moves


## Minimax Termination Modes


## Zobrist Manager


### Zobrist Component


### Transposition Table


<div class="section_buttons">

| Previous          |                              Next |
|:------------------|----------------------------------:|
| [Components](05_components.md) | [Engine Data](07_game_data.md) |

</div>