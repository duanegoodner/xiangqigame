# Minimax Details  

The heavy-lifting in our AI engine is handled by the moveselection::MinimaxMoveEvaluator class, an implementation of the MoveEvaluator interface. The interface requires implementation of a single method: ImplementMoveSelection which takes a gameboard::MoveCollection of available moves as its only argument, and returns a single gameboard::Move selected from the available moves. 

## Algorithm Termination Modes

When moveselection::MinimaxMoveEvaluator.ImplementSelectMove is called, game state nodes in the game tree are recursively evaluated to a tree depth specified by moveselection::MinimaxMoveEvaluator.search_depth_. For each visited node, the evaluator determines an integer "points" value, corresponding to how favorable or unfavorable a particular game state is for the evaluating player.

The search down a particular tree branch stops when any of the the following conditions is met:

- An entry for the node's board state already exists in the evaluator's boardstate::TranspositionTable
- The node is an end game condition leaf (either a win, loss, or draw)
- The maximum search depth has been reached
- An [alpha/beta pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) criteria is satisfied
  

## Points Assignment

Points values of non-leaf nodes, are determined based on the points values of their child nodes. For end-of-game leaf nodes, negative infinity is assigned if the node represents a loss for the evaluator, positive infinity is assigned for a win, and zero is assigned for a draw. When evaluating any other leaf node, we do the following:

- For each piece on the gameboard::GameBoard, look up the element in the evalutor's 4-D piecepoints::PiecePositionPoints.points_array of integers corresponding to the piece's gameboard::PieceColor, gameboard::PieceType, gameboard::BoardSpace.rank, and gameboard::BoardSpace.file.

- Calculate the the "score" of the game state node as the sum of entries for each piece on the board

The entries in piecepoints::PiecePositionPoints.points_array are the same as those used in [Yen, Shi-Jim, et al. "Computer chinese chess." _ICGA journal_ 27.1 (2004): 3-18.](http://web.csie.ndhu.edu.tw/sjyen/Papers/2004CCC.pdf)

  
  

## Pre-Search Sorting of Available Moves

Prior to evaluating the children of any node, the child nodes are sorted based on the (easy to calculate) non-recursive points associated with each child game state in order from best to worst for the moving player. By iterating over the child nodes in this order for the recursive search, the likelihood of meeting the alpha-beta pruning criteria, and thus being able to skip evaluation of some child nodes, increases significantly.

  

## Zobrist Coordinator

An evaluator's boardstate::ZobristCoordinator coordinates the calculation of [Zobrist hash values](https://en.wikipedia.org/wiki/Zobrist_hashing) of each encountered board state, as well as the storage/retrieval of results from each call to moveselection::MinimaxMoveEvaluator.MinimaxRecursive in a boardstate::TranspositionTable.

  

The coordinator's boardstate::ZobristCoordinator.zobrist_component_ contains one or more boardstate::ZobristCalculator objects. The boardstate::ZobristComponent.primary_calculator_ is used to calculate the Zobrist hash values that are used as keys in boardstate::TranspositionTable.data_ (a std::unordered_map). Each value in a boardstate::TranspositionTable.data_ entry is the result of a call to moveselection::MinimaxMoveEvaluator.MinimaxRecursive.

  

The moveselection::ZobristCoordinator.confirmation_keys_ attribute is a std::array that can hold zero or more boardstate::ZobristCalculator objects. If this array is non-empty, the boardstate::ZobristCalculator objects in it are used to calculate "backup" hash values which are included in the boardstate::TranspositionTableEntry values of boardstate::TranspositionTable.data_ and are then used to detect hash collisions in the boardstate::TranspositionTable. For sufficiently large hash key sizes, using one or more boardstate::ZobristCalculator in moveselection::ZobristCoordinator.confirmation_keys_ can reliably detect the existence of any hash collision.

  

With two or more elements in moveselection::ZobristCoordinator.confirmation_keys_, it would be possible to correct (i.e. avoid any confounding due to) a collision. Currently, our AI engine contains methods for detecting collisions when we have one or more backup calculators, but does not implement any method for "correcting" misinformation caused by a collision.

  

## Performance 

Using an Intel i7-13700K CPU, the average evaluation time per node of moveselection::MinimaxMoveEvaluator is ~15 ns. With an evaluator's search depth set to 7, the maximum number of nodes visited for any single move selection during the course of a game is ~ 2e7 nodes. With a shallower search depth of just three layers in the game tree, this value drops to ~5e3 nodes.

  

## Possible Improvements  

### A Smarter Evaluator

Our heuristic-based approach to node evaluation with constant maximum search depth throughout the game is fairly simple and can almost certainly be improved. Potential paths for improvement include the use of iterative deepening, quiescent searches, Monte Carlo tree searches, and deep learning based implementations of the PiecePoints and/or MoveEvaluator interfaces. In many cases, implementing extensions/improvements could be relatively straightforward due to the plug-in architecture of our AI engine.

### Speed Improvements

By using C++ (instead of a higher level language) for the core engine, and taking some care in design decisions (e.g. compile time polymorphism for interfaces), we have achieved decent performance in Xiangqigame, but there is certainly room for improvement. moveselection::MinimaxMoveEvaluator, along with most other parts of the AI engine has not been extensively profiled. Running performance profiling would likely reveal bottlenecks that can be improved.

Currently, the recursive Minimax algorithm runs on a single thread. Incorporating some parallelism into the engine with a tool such as [OpenMP tasks](https://www.openmp.org/wp-content/uploads/OpenMP-UMT-Tasking-1.pdf) would provide another path for speed improvements.
 

### Reduce Python Responsibilities


  

<div  class="section_buttons">


| Previous | Next |
|:------------------|----------------------------------:|
| [Components](05_components.md) | [Engine Data](07_game_data.md) |

</div>