


# Terminal Game Board and Notation

The following screenshot shows the state of the terminal UI after a human red player has executed a single opening move.

<div style="text-align:left">
  <img src="../resources/board_for_notation_section.png" alt="Alt text" width="700">
</div>



## Piece Notation

In our simple text-based game board, each piece is represented by two characters. The first character represents the piece type, and the second character represents the color.

#### First character (piece type)
- S = Soldier
- C = Cannon
- R = Chariot
- H = Horse
- E = Elephant
- A = Advisor
- G = General

#### Second character (piece color)
- r = Red
- b = Black

Unoccupied spaces are represented as `--`


### Board Space Notation

The terminal UI uses algebraic notation for board spaces. Files are represented by letters **a** through **i**, ranks are represented numbers **1** through **10**. Red's General starts in rank **1**, and Black's General starts in rank 10. File **a** corresponds to Red's left-most file. To represent a specific board space, we write the file letter followed by the rank number, with no spaces in between. For example, the starting position of Black's general is **e10**.


### Move Notation

The syntax for a move is: starting-board-space, ending-board-space. In the example board shown above, Red has just opened the game with: **b1, c3**


### Board Features

Although castles and rivers are not marked on our simple text game board, the core engine performs all calculations with these features in their standard locations. Using our algebraic notation, the river edges are located at ranks **5** and **6**, 

#### River Edges
- Red Side = rank **5**
- Black Side = rank **6**


#### Castle Spaces
- Red = d1, e1, f1, d2, e2, f2, d3, e3, f3
- Black: d10, e10, f10, d9, e9, f9, d8, e8, f8


