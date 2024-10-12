# Components


## UML Diagram


A UML diagram showing the C++ namespaces and classes can be viewed [here](../../plantuml/xiangqigame_core.svg). Key things to note in this diagram are:
- There 5 non-anonymous namespaces: gameboard, boardstate, piecepoints, moveslection, and jsonio
- There are 5 anonymously namespaced classes: SpaceInfoProvider, PieceValueProvider, MoveEvaluator, JsonUtility, and BoardStateSummarizer.
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

Tracks the gameboard::BoardSpace locations of gameboard::GamePiece objects. With the help of its gameboard::PieceMoves member, determines the legal moves, and provides this infomrmation to a MoveEvaluator. nforms a MoveEvaluator of the locatione gameboard::GamePiece objects, and what gameboard::Moves are legal from a particular configuration. Implementatiof move rules (gameboard::PieceMoves) is an attribute of the concrete gameboard::GameBoard class.

### BoardStateSummarizer (hashcalculator)


### MoveEvaluator (moveselection)




