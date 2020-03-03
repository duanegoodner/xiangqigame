# portfolio-project

Write a class named XiangqiGame for playing an abstract board game called xiangqi.   Please read the "Board", "Rules", and "Pieces" sections on [the Wikipedia page](https://en.wikipedia.org/wiki/Xiangqi).  You do not have to implement the rules regarding perpetual check or chasing.  You **do** need to correctly handle stalemate.  You also need to correctly handle all piece-specific rules, e.g. generals aren't allowed to "see" each other, horses can be blocked, elephants can't cross the river, etc.

A general is in check if it could be captured on the opposing player's next move. A player cannot make a move that puts or leaves their general in check. The Wikipedia page says "The game ends when one player captures the other's general", but it's more accurate to say that it ends when one player **checkmates** the other's general.  You don't actually capture a general, instead you have to put it in such a position that it cannot escape being in check, meaning that no matter what, it could be captured on the next move.  This works the same as in chess, if you're familiar with that game.

Red is the starting player.

Locations on the board will be specified using "algebraic notation", with columns labeled a-i and rows labeled 1-10, with row 1 being the Red side and row 10 the Black side.

You're not required to print the board, but you will probably find it very useful for testing purposes.

Your XiangqiGame class must include the following:
* An init method that initializes any data members.
* A method called get_game_state that just returns 'UNFINISHED', 'RED_WON' or 'BLACK_WON'.
* A method called is_in_check that takes as a parameter either 'red' or 'black' and returns True if that player is in check, but returns False otherwise.
* A method called make_move that takes two parameters - strings that represent the square moved from and the square moved to.  For example, make_move('b3', 'b10').  If the square being moved from does not contain a piece belonging to the player whose turn it is, or if the indicated move is not legal, or if the game has already been won, then it should just return False.  Otherwise it should make the indicated move, remove any captured piece, update the game state if necessary, update whose turn it is, and return True.

Feel free to add whatever other classes, methods, or data members you want.  All data members must be private.  Every class should have an init method that initializes all of the data members for that class.

Here's a very simple example of how the class could be used:
```
game = XiangqiGame()
move_result = game.make_move('c1', 'e3')
black_in_check = game.is_in_check('black')
game.make_move('e7', 'e6')
state = game.get_game_state()
```
The file must be named: **XiangqiGame.py**
