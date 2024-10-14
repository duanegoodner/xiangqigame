# Components


## UML Diagram


Relationships among classes and the organization of classes into namespaces can be viewed in the [UML diagram linked here](../../plantuml/xiangqigame_core.svg). Key things to note in this diagram are:
- There five non-anonymous namespaces: gameboard, boardstate, piecepoints, moveslection, and jsonio
- There are five anonymously namespaced classes: SpaceInfoProvider, PieceValueProvider, MoveEvaluator, JsonUtility, and BoardStateSummarizer.
- Each of the anonymously namespaced classes is a CRTP interface and is implemented by one or two concrete classes from exactly one of the 5 namespaces.
- The concrete-to-interface class pairings are:
    - gameboard::GameBoard implements SpaceInfoProvider.
    - boardstate::HashCalculator implements BoardStateSummarizer.
    - piecepoints::PiecePositionPoints implements PieceValueProvider.
    - moveselection::MinimaxMoveEvaluator and moveselection::RandomMoveEvaluator each implement MoveEvaluator.
    - jsonio::NlohmannJsonUtility implements JsonUtility.


## Component Descriptions

Each namespace, combined with the interface class it is paired with, can be considered a component of the AI engine. The primary responsibilities of each component are described below. In general, the class structure aims to prevent the engine's highest level concept - move selection - from coupling with implementation details of lower level components.

### jsonio + JsonInterface

Imports .json files to C++ objects such as a piecepoints::BPOPositionPoints. Currently, the sole implementation of the JsonUtility interface uses the [Nlohmann json library](https://github.com/nlohmann/json). The interface keeps the rest of the codebase isolated from the details of the external library. If we ever want to use an additional json library and/or replace the Nlohmann, having the JsonInterface would simplify the integration process.


### piecepoints + PieceValueProvider

Informs some implementations of a MoveEvaluator with the value associated with having a particular piece located at a particular gameboard::BoardSpace.

### gameboard + SpaceInfoProvider

Tracks the gameboard::BoardSpace locations of gameboard::GamePiece objects, and determines which moves of gameboard::GamePiece objects are allowed. Our concrete implementation of SpaceInforProvider, gameboard::GameBoard, accomplishes these tasks with the help of its gameboard::GameBoard. With the help of its gameboard::GameBoard.board_map_ and a gameboard::MoveCalculator.

Within the gameboard namespace, all [standard rules of Xiangi](https://en.wikipedia.org/wiki/Xiangqi#Rules) are implemented, except for the following:

- There is no mechanism for declaring a draw.
- Repeat move patterns with a period longer than 4 are NOT prohibited.
  - ABABAB, ABCABCABC, and ABCDABCDABCD are prohibited
  - Patterns of the form ABCD[X]ABCD[X]ABCD[X] are not prohibited



### hashcalculator + BoardStateSummarizer
Holds an integer that represents the current board state, and updates the value of this board state integer whenever the gameboard::GamePiece configuration of a gameboard::GameBoard changes. Provides a MoveEvaluator with fast access to storage and retrieval of calculation results in a data structrue that usese board state integer values as keys.

hashcalculator::HashCalculator, our concrete BoardStateSummarizer, supports using either a 64-bit or 128-bit integer for its hashcalculator::HashCalculator.board_state_. The board state value is updated using a hashcalculator::ZobristKeys object combined with information from a gameboard::ExecutedMove. 



### MoveEvaluator (moveselection)


<div class="section_buttons">

| Previous          |                              Next |
|:------------------|----------------------------------:|
| [Running](05_game_data.md) |                          |

</div>




