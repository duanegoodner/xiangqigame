# Components


## UML Diagram


Relationships among classes and the organization of classes into namespaces can be viewed in the [UML diagram linked here](../../plantuml/xiangqigame_core.svg). Key things to note in this diagram are:
- There five namespaces: gameboard, boardstate, piecepoints, moveslection, and jsonio
- There are five anonymously namespaced classes: SpaceInfoProvider, PieceValueProvider, MoveEvaluator, JsonUtility, and BoardStateCoordinator.
- Each of the anonymously namespaced classes is a CRTP interface and is implemented by one or two concrete classes from exactly one of the 5 namespaces.
- The concrete-to-interface class pairings are:
    - gameboard::GameBoard implements SpaceInfoProvider.
    - boardstate::ZobristCoordinator implements BoardStateCoordinator.
    - piecepoints::PiecePositionPoints implements PieceValueProvider.
    - moveselection::MinimaxMoveEvaluator and moveselection::RandomMoveEvaluator each implement MoveEvaluator.
    - jsonio::NlohmannJsonUtility implements JsonUtility.


## Components

Each namespace, combined with the interface class it is paired with, can be considered a component of the AI engine. The primary responsibilities of each component are described below. In general, the class structure aims to prevent the engine's highest level concept - move selection - from coupling with implementation details of lower level components.

### JsonIO

**JsonUtilityInterface + jsonio namespace**: Imports .json files to C++ objects such as a piecepoints::BPOPositionPoints. Currently, the sole implementation of the JsonUtility interface uses the [Nlohmann json library](https://github.com/nlohmann/json). The interface keeps the rest of the codebase isolated from the details of the external library. If we ever want to use an additional json library and/or replace the Nlohmann, having the JsonInterface would simplify the integration process.


### PiecePoints

**PieceValueProvider + piecepoints namespace**: Informs some implementations of a MoveEvaluator with the value associated with having a particular piece located at a particular gameboard::BoardSpace.

### GameBoard

**SpaceInfoProvider + gameboard namespace**: Tracks the gameboard::BoardSpace locations of gameboard::GamePiece objects, and determines which moves of gameboard::GamePiece objects are allowed. Our concrete implementation of SpaceInforProvider, gameboard::GameBoard, accomplishes these tasks with the help of its gameboard::GameBoard. With the help of its gameboard::GameBoard.board_map_ and a gameboard::MoveCalculator.

Within the gameboard namespace, all [standard rules of Xiangi](https://en.wikipedia.org/wiki/Xiangqi#Rules) are implemented, execept for some of the rules related to repeated moves. Specifically:
- The perpetual checking and perpetual chasing rules are NOT implemented
- Triple repeat move patterns with a periods of 2, 3, and 4 ARE prohibited (ABABAB, ABCABCABC, and ABCDABCDABCD are prohibited).
- Triple repeat patterns with a period longer than 4 are NOT prohibited (ABCDEABCDEABCDE, and anything else of the form [X]ABCD[X]ABCD[X] is NOT prohibited).


### BoardState 
**BoardStateCoordinator + boardstate namespace**: Maintains an integer that represents the current board state, and updates the value of this board state integer whenever the gameboard::GamePiece configuration of a gameboard::GameBoard changes. Provides a MoveEvaluator with fast access to storage and retrieval of calculation results in a data structrue that usese board state integer values as keys.

boardstate::ZobristCoordinator, our concrete BoardStateCoordinator, supports using either a 32-bit, 64-bit, or 128-bit integer for its hashcalculator::HashCalculator.board_state_. The board state value is updated using a hashcalculator::ZobristKeys object combined with information from a gameboard::ExecutedMove. 


### MoveEvaluator 
**MoveEvaluator + moveselection namespace**: Selects a gameboard::Move to execute. The moveselection::MinimaxMoveEvaluator implementation uses the recursive Minimax algorithm with the value of a particular board configuration determined based on piece- and position- dependent points obtained from a PieceValueProvider. This act of move selection represents the highest level concept on the C++ side of the project.


<div class="section_buttons">

| Previous          |                              Next |
|:------------------|----------------------------------:|
| [Notation](04_notation.md) | [Minimax Details](06_minimax_details.md) |

</div>




