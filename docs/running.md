# Running (playing the game)

Once `xiangqigame` has been installed into a Python environment, games can be run using the command `play_xiangqi`.

## Command line options

Information on all available `play_xiangqi` options can be obtained by running `play_xiangqi --help`.


```shell
usage: play_xiangqi [-h] [-rt {person,ai}] [-ra {random,minimax}] [-rst {1,2,3,4,5,6,7,8,9}]
                    [-rk {64,128}] [-bt {person,ai}] [-ba {random,minimax}]
                    [-bst {1,2,3,4,5,6,7,8,9}] [-bk {64,128}] [-s] [-d OUTPUT_DIR_SUFFIX]

A Xiangqi (a.k.a. Chinese Chess) game that can be played as Human vs. Human, AI vs. AI, or Human
vs. AI

options:
  -h, --help            show this help message and exit
  -rt {person,ai}, --red_player_type {person,ai}
                        Can be 'person', or 'ai'. Default is 'ai'.
  -ra {random,minimax}, --red_algo {random,minimax}
                        Search algorithm to use for red player (if player type is 'ai'). Can be
                        'random' or 'minimax'. Default is minimax.
  -rst {1,2,3,4,5,6,7,8,9}, --red_strength {1,2,3,4,5,6,7,8,9}
                        Search depth to user for red AI player with minimax algoDefault is 4.
  -rk {64,128}, --red_key_size {64,128}
                        Key size (in bits) used for red AI player Zobrist hashing
  -bt {person,ai}, --black_player_type {person,ai}
                        Can be 'person', or 'ai'. Default is 'ai'.
  -ba {random,minimax}, --black_algo {random,minimax}
                        Search depth to user for black AI player with minimax algoDefault is 4.
  -bst {1,2,3,4,5,6,7,8,9}, --black_strength {1,2,3,4,5,6,7,8,9}
                        Search depth to user for red player when red is 'ai' with 'minimax.'
                        Default is 4.
  -bk {64,128}, --black_key_size {64,128}
                        Key size (in bits) used for black AI player Zobrist hashing
  -s, --save_summary    Save GameSummary as .json
  -d OUTPUT_DIR_SUFFIX, --output_dir_suffix OUTPUT_DIR_SUFFIX
                        String to append to end of output directory name. Output dir relative to
                        cwd will be ./data/game_summaries/<timestamp><optional-suffix>

Note: If all default parameters are used, both players will be AI and use Minimax with search depth
= 4, and 64 bit Zobrist keys.
```

## Example Commands with Links to Demo of Resulting Games

The following list shows examples of the `play_xiangqi` command. Each example command is linked to a terminal recording of a game that resulted from running that command.

- [`play_xiangqi -rst 5 -bst 5 -s -d game_01`](demos/game_01.md)
- [`play_xiangqi -rst 5 -bst 3 -s -d game_02`](demos/game_02.md)
- [`play_xiangqi -rst 3 -bst 5 -s -d game_03`](demos/game_03.md)
- [`play_xiangqi -ra random -bst 5 -s -d game_04`](demos/game_04.md)
- [`play_xiangqi -rst 6 -bst 6 -s -d game_05`](demos/game_05.md)
- [`play_xiangqi -rst 5 -bst 5 -bk 128 -s -d game_06`](demos/game_06.md)
- [`play_xiangqi -rt person -bst 4 -s -d game_07`](demos/game_07.md)
