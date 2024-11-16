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
      [ "The play_xiangqi Command", "md_docs_03_running.html#autotoc_md19", null ],
      [ "Example Games", "md_docs_03_running.html#autotoc_md20", [
        [ "Game #1:", "md_docs_03_running.html#autotoc_md21", [
          [ "Settings", "md_docs_03_running.html#autotoc_md22", null ],
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md23", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md24", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md25", null ]
        ] ],
        [ "Game #2:", "md_docs_03_running.html#autotoc_md26", [
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md27", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md28", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md29", null ]
        ] ],
        [ "Game #3:", "md_docs_03_running.html#autotoc_md30", [
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md31", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md32", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md33", null ]
        ] ],
        [ "Game #4:", "md_docs_03_running.html#autotoc_md34", [
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md35", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md36", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md37", null ]
        ] ],
        [ "Game #5:", "md_docs_03_running.html#autotoc_md38", [
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md39", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md40", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md41", null ]
        ] ],
        [ "Game #6", "md_docs_03_running.html#autotoc_md42", [
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md43", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md44", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md45", null ]
        ] ],
        [ "Game #7", "md_docs_03_running.html#autotoc_md46", [
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md47", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md48", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md49", null ]
        ] ]
      ] ]
    ] ],
    [ "Terminal Game Board and Notation", "md_docs_04_notation.html", [
      [ "Piece Notation", "md_docs_04_notation.html#autotoc_md51", [
        [ "Board Space Notation", "md_docs_04_notation.html#autotoc_md54", null ],
        [ "Move Notation", "md_docs_04_notation.html#autotoc_md55", null ],
        [ "Board Features", "md_docs_04_notation.html#autotoc_md56", [
          [ "First character (piece type)", "md_docs_04_notation.html#autotoc_md52", null ],
          [ "Second character (piece color)", "md_docs_04_notation.html#autotoc_md53", null ],
          [ "River Edges", "md_docs_04_notation.html#autotoc_md57", null ],
          [ "Castle Spaces", "md_docs_04_notation.html#autotoc_md58", null ]
        ] ]
      ] ]
    ] ],
    [ "Components", "md_docs_05_components.html", [
      [ "UML Diagram", "md_docs_05_components.html#autotoc_md60", null ],
      [ "Components", "md_docs_05_components.html#autotoc_md61", [
        [ "JsonIO", "md_docs_05_components.html#autotoc_md62", null ],
        [ "PiecePoints", "md_docs_05_components.html#autotoc_md63", null ],
        [ "GameBoard", "md_docs_05_components.html#autotoc_md64", null ],
        [ "BoardState", "md_docs_05_components.html#autotoc_md65", null ],
        [ "MoveEvaluator", "md_docs_05_components.html#autotoc_md66", null ]
      ] ]
    ] ],
    [ "Minimax Details", "md_docs_06_minimax_details.html", null ],
    [ "AI Engine Data", "md_docs_07_game_data.html", null ],
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
"classboardstate_1_1ZobristCoordinator.html#a45572ae707decd032c77c9c216c83b2e",
"classmoveselection_1_1MinimaxMoveEvaluator.html#afdf74a02920f5037e16d90211cf25f92",
"classxiangqipy_1_1core__dataclass__mirrors_1_1BoardSpace.html#a65019c92f1e797804d9867a0f9d7531c",
"classxiangqipy_1_1game__summary__plot__manager_1_1GameSummaryPlotManager.html",
"classxiangqipy_1_1players_1_1IllegalMoveInMoveList.html#a161e0c283043b872ed5ed421fd425f3f",
"game__summary__io_8py.html#a7d882e81ad5f3acd948dbc2d1f862fae",
"namespacemoveselection_1_1minimaxutils.html#af8f5372c781c8f3203e21ba882663241",
"structgameboard_1_1MoveCollection.html#a40c1b1e41cb2452431b77b9c3cc0e4c1"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';