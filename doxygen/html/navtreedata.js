/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Xiangiqgame", "index.html", [
    [ "Overview", "index.html#autotoc_md0", [
      [ "Core Algorithm", "index.html#autotoc_md1", null ],
      [ "Architecture", "index.html#autotoc_md2", null ],
      [ "Integration with Python", "index.html#autotoc_md3", null ],
      [ "Engine Settings and Data Collection", "index.html#autotoc_md4", [
        [ "AI engine parameters that can be modified via the command line interface:", "index.html#autotoc_md5", null ],
        [ "Data collected each time an AI player selectes a move include:", "index.html#autotoc_md6", null ]
      ] ]
    ] ],
    [ "Installing", "md_docs_02_installing.html", [
      [ "Native Installation", "md_docs_02_installing.html#autotoc_md8", [
        [ "Requirements", "md_docs_02_installing.html#autotoc_md9", null ],
        [ "Procedure", "md_docs_02_installing.html#autotoc_md10", null ],
        [ "Native Installation Demo", "md_docs_02_installing.html#autotoc_md11", null ]
      ] ],
      [ "Docker Installation", "md_docs_02_installing.html#autotoc_md12", [
        [ "Requirements", "md_docs_02_installing.html#autotoc_md13", null ],
        [ "Procedure", "md_docs_02_installing.html#autotoc_md14", null ],
        [ "Stopping the Docker Container", "md_docs_02_installing.html#autotoc_md15", null ],
        [ "Re-Starting the Docker Container", "md_docs_02_installing.html#autotoc_md16", null ],
        [ "Demo", "md_docs_02_installing.html#autotoc_md17", null ]
      ] ]
    ] ],
    [ "Running", "md_docs_03_running.html", [
      [ "The play_xiangqi Command", "md_docs_03_running.html#autotoc_md19", [
        [ "Terminal Recording of Game with Default Settings", "md_docs_03_running.html#autotoc_md20", null ],
        [ "Additional Example Games", "md_docs_03_running.html#autotoc_md21", null ]
      ] ]
    ] ],
    [ "Terminal Game Board and Notation", "md_docs_04_notation.html", [
      [ "Piece Notation", "md_docs_04_notation.html#autotoc_md23", [
        [ "Board Space Notation", "md_docs_04_notation.html#autotoc_md26", null ],
        [ "Move Notation", "md_docs_04_notation.html#autotoc_md27", null ],
        [ "Board Features", "md_docs_04_notation.html#autotoc_md28", [
          [ "First character (piece type)", "md_docs_04_notation.html#autotoc_md24", null ],
          [ "Second character (piece color)", "md_docs_04_notation.html#autotoc_md25", null ],
          [ "River Edges", "md_docs_04_notation.html#autotoc_md29", null ],
          [ "Castle Spaces", "md_docs_04_notation.html#autotoc_md30", null ]
        ] ]
      ] ]
    ] ],
    [ "Components", "md_docs_05_components.html", [
      [ "UML Diagram", "md_docs_05_components.html#autotoc_md32", null ],
      [ "Components", "md_docs_05_components.html#autotoc_md33", [
        [ "JsonIO", "md_docs_05_components.html#autotoc_md34", null ],
        [ "PiecePoints", "md_docs_05_components.html#autotoc_md35", null ],
        [ "GameBoard", "md_docs_05_components.html#autotoc_md36", null ],
        [ "BoardState", "md_docs_05_components.html#autotoc_md37", null ],
        [ "MoveEvaluator", "md_docs_05_components.html#autotoc_md38", null ]
      ] ]
    ] ],
    [ "Minimax Details", "md_docs_06_minimax_details.html", [
      [ "Algorithm Termination Modes", "md_docs_06_minimax_details.html#autotoc_md40", null ],
      [ "Points Assignment", "md_docs_06_minimax_details.html#autotoc_md41", null ],
      [ "Pre-Search Sorting of Available Moves", "md_docs_06_minimax_details.html#autotoc_md42", null ],
      [ "Zobrist Coordinator", "md_docs_06_minimax_details.html#autotoc_md43", null ],
      [ "Performance", "md_docs_06_minimax_details.html#autotoc_md44", null ],
      [ "Possible Improvements", "md_docs_06_minimax_details.html#autotoc_md45", [
        [ "A Smarter Evaluator", "md_docs_06_minimax_details.html#autotoc_md46", null ],
        [ "Speed Improvements", "md_docs_06_minimax_details.html#autotoc_md47", null ],
        [ "Reduce Python Responsibilities", "md_docs_06_minimax_details.html#autotoc_md48", null ]
      ] ]
    ] ],
    [ "AI Engine Data", "md_docs_07_game_data.html", [
      [ "GameSummary", "md_docs_07_game_data.html#autotoc_md50", [
        [ "Auto-saving Game Summary Data", "md_docs_07_game_data.html#autotoc_md51", null ],
        [ "Auto-generating Plots", "md_docs_07_game_data.html#autotoc_md52", [
          [ "Minimax Node Counts by Search  Result Type", "md_docs_07_game_data.html#autotoc_md53", null ],
          [ "Move Selection Time", "md_docs_07_game_data.html#autotoc_md54", null ],
          [ "Minimax Evaluation Scores", "md_docs_07_game_data.html#autotoc_md55", null ],
          [ "Data from Additional Games", "md_docs_07_game_data.html#autotoc_md56", null ]
        ] ],
        [ "Additional / Custom Analysis of GameSummary Data", "md_docs_07_game_data.html#autotoc_md57", null ]
      ] ]
    ] ],
    [ "Example Games", "md_docs_08_example_games.html", null ],
    [ "Todo List", "todo.html", null ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ],
        [ "Enumerator", "namespacemembers_eval.html", null ]
      ] ]
    ] ],
    [ "Concepts", "concepts.html", "concepts" ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", "functions_vars" ],
        [ "Typedefs", "functions_type.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Variables", "globals_vars.html", null ],
        [ "Typedefs", "globals_type.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"____init_____8py.html",
"classboardstate_1_1TranspositionTablePruner.html#a87ad626687395271c07b9225fdee2be4",
"classboardstate_1_1ZobristCoordinator.html#afd4006b2ff13a627eb719c3d5421af63",
"classgameboard_1_1GameBoardForConcepts.html#a6baf7e6d5d716702318b82283052dcee",
"classmoveselection_1_1MinimaxMoveEvaluatorFactory.html",
"classmoveselection_1_1TranspositionTableSearchResult.html#af623237380b07e2c786d08341d6622c9",
"classxiangqipy_1_1command__input_1_1PlayerCommandInterpreter.html#a794ae949ce28e00580a45b2f41944e5f",
"classxiangqipy_1_1game__interfaces_1_1MoveReporter.html#ace2963ee4e3442dd85fdb27862fab31d",
"classxiangqipy_1_1player__summary_1_1PlayerSummary.html#a122ccb24a667bc628478c88326fa2da7",
"functions_func_n.html",
"move__calculator__details_8hpp.html",
"namespacexiangqipy_1_1handlers_1_1errors.html",
"structgameboard_1_1MoveCollection.html#a40c1b1e41cb2452431b77b9c3cc0e4c1"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';