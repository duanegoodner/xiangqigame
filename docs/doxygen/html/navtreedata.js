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
    [ "Overview", "index.html", null ],
    [ "Installing Xiangqigame", "md_docs_installing.html", [
      [ "Native Installation", "md_docs_installing.html#autotoc_md1", [
        [ "Requirements", "md_docs_installing.html#autotoc_md2", null ],
        [ "Procedure", "md_docs_installing.html#autotoc_md3", null ],
        [ "Demo", "md_docs_installing.html#autotoc_md4", null ]
      ] ],
      [ "Docker Installation", "md_docs_installing.html#autotoc_md5", [
        [ "Requirements", "md_docs_installing.html#autotoc_md6", null ],
        [ "Procedure", "md_docs_installing.html#autotoc_md7", null ],
        [ "Stopping the Docker Container", "md_docs_installing.html#autotoc_md8", null ],
        [ "Re-Starting the Docker Container", "md_docs_installing.html#autotoc_md9", null ],
        [ "Demo", "md_docs_installing.html#autotoc_md10", null ]
      ] ]
    ] ],
    [ "Running (playing the game)", "md_docs_running.html", [
      [ "The <tt>play_xiangqi</tt> Command", "md_docs_running.html#autotoc_md12", null ],
      [ "Example Games", "md_docs_running.html#autotoc_md13", [
        [ "Red=SD5, Black=SD5", "md_docs_running.html#autotoc_md14", [
          [ "Terminal Recording", "md_docs_running.html#autotoc_md15", null ],
          [ "Minimax Data", "md_docs_running.html#autotoc_md16", null ],
          [ "Synopsis", "md_docs_running.html#autotoc_md17", null ]
        ] ],
        [ "Red=SD5, Black=SD3", "md_docs_running.html#autotoc_md18", [
          [ "Terminal Recording", "md_docs_running.html#autotoc_md19", null ],
          [ "Minimax Data", "md_docs_running.html#autotoc_md20", null ],
          [ "Synopsis", "md_docs_running.html#autotoc_md21", null ]
        ] ],
        [ "Red=SD3, Black=SD5", "md_docs_running.html#autotoc_md22", [
          [ "Terminal Recording", "md_docs_running.html#autotoc_md23", null ],
          [ "Minimax Data", "md_docs_running.html#autotoc_md24", null ],
          [ "Synopsis", "md_docs_running.html#autotoc_md25", null ]
        ] ],
        [ "Red=Random, Black=SD5", "md_docs_running.html#autotoc_md26", [
          [ "Terminal Recording", "md_docs_running.html#autotoc_md27", null ],
          [ "Minimax Data", "md_docs_running.html#autotoc_md28", null ],
          [ "Synopsis", "md_docs_running.html#autotoc_md29", null ]
        ] ],
        [ "Red=SD6, Black=SD6", "md_docs_running.html#autotoc_md30", [
          [ "Terminal Recording", "md_docs_running.html#autotoc_md31", null ],
          [ "Minimax Data", "md_docs_running.html#autotoc_md32", null ],
          [ "Synopsis", "md_docs_running.html#autotoc_md33", null ]
        ] ],
        [ "Red=SD5:K64, Black=SD5:K128", "md_docs_running.html#autotoc_md34", [
          [ "Terminal Recording", "md_docs_running.html#autotoc_md35", null ],
          [ "Minimax Data", "md_docs_running.html#autotoc_md36", null ],
          [ "Synopsis", "md_docs_running.html#autotoc_md37", null ]
        ] ],
        [ "Red=Human, Black=SD4", "md_docs_running.html#autotoc_md38", [
          [ "Terminal Recording", "md_docs_running.html#autotoc_md39", null ],
          [ "Minimax Data", "md_docs_running.html#autotoc_md40", null ],
          [ "Synopsis", "md_docs_running.html#autotoc_md41", null ]
        ] ]
      ] ]
    ] ],
    [ "Terminal Game Board and Notation", "md_docs_notation.html", null ],
    [ "Implemented Rules", "md_docs_implemented_rules.html", null ],
    [ "Design and Implementation Highlights", "md_docs_implementation_details.html", [
      [ "Top Level Components", "md_docs_implementation_details.html#autotoc_md53", null ],
      [ "C++ Core Engine", "md_docs_implementation_details.html#autotoc_md54", [
        [ "Libraries", "md_docs_implementation_details.html#autotoc_md55", null ],
        [ "Internal Game Board", "md_docs_implementation_details.html#autotoc_md56", null ],
        [ "Minimax Algorithm", "md_docs_implementation_details.html#autotoc_md57", [
          [ "Piece Points Values", "md_docs_implementation_details.html#autotoc_md58", null ],
          [ "Board Configuration Scores", "md_docs_implementation_details.html#autotoc_md59", null ],
          [ "Alpha-Beta Pruning", "md_docs_implementation_details.html#autotoc_md60", null ],
          [ "Pre-search sorting", "md_docs_implementation_details.html#autotoc_md61", null ],
          [ "Zobrist Hasing", "md_docs_implementation_details.html#autotoc_md62", null ],
          [ "Transposition Tables", "md_docs_implementation_details.html#autotoc_md63", null ],
          [ "64 bit vs 128 bit hash keys", "md_docs_implementation_details.html#autotoc_md64", null ]
        ] ],
        [ "CRTP Class Interfaces", "md_docs_implementation_details.html#autotoc_md65", null ]
      ] ],
      [ "Pybind11 Module", "md_docs_implementation_details.html#autotoc_md66", [
        [ "MinimaxMoveEvaluator", "md_docs_implementation_details.html#autotoc_md67", null ],
        [ "RandomMoveEvaluator", "md_docs_implementation_details.html#autotoc_md68", null ],
        [ "SearchSummary", "md_docs_implementation_details.html#autotoc_md69", null ],
        [ "BoardSpace", "md_docs_implementation_details.html#autotoc_md70", null ],
        [ "GamePiece", "md_docs_implementation_details.html#autotoc_md71", null ],
        [ "Move", "md_docs_implementation_details.html#autotoc_md72", null ],
        [ "MoveCollection", "md_docs_implementation_details.html#autotoc_md73", null ],
        [ "BestMoves", "md_docs_implementation_details.html#autotoc_md74", null ],
        [ "ExecutedMove", "md_docs_implementation_details.html#autotoc_md75", null ],
        [ "PieceColor", "md_docs_implementation_details.html#autotoc_md76", null ],
        [ "PieceType", "md_docs_implementation_details.html#autotoc_md77", null ],
        [ "size_of_points_type", "md_docs_implementation_details.html#autotoc_md78", null ],
        [ "is_signed_points_type", "md_docs_implementation_details.html#autotoc_md79", null ],
        [ "MinimaxResultType", "md_docs_implementation_details.html#autotoc_md80", null ],
        [ "GameBoard", "md_docs_implementation_details.html#autotoc_md81", null ]
      ] ],
      [ "Python Package Responsibilities", "md_docs_implementation_details.html#autotoc_md82", [
        [ "Players", "md_docs_implementation_details.html#autotoc_md83", null ],
        [ "Game", "md_docs_implementation_details.html#autotoc_md84", null ],
        [ "Packaging", "md_docs_implementation_details.html#autotoc_md85", null ],
        [ "UI", "md_docs_implementation_details.html#autotoc_md86", null ]
      ] ]
    ] ],
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
        [ "Variables", "functions_vars.html", null ],
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
"classmoveselection_1_1MinimaxMoveEvaluator.html#ae691602ed173beb3246329f714611980",
"classxiangqipy_1_1game__interfaces_1_1Player.html#a803f419269cdc649f9dcbe4a75b34d9d",
"classxiangqipy_1_1players_1_1HumanPlayer.html#a1378c449f1fd2534ba27ebad160056a7",
"game__summary__plot__manager_8py.html#ad3e2484169388585491c6fdb5bf935d0",
"namespaceutility__functs.html#aca0e7a98a98ae175c40f149c411fad38",
"structmoves_1_1MoveCollection.html#a841da3a4de16c7b925970ae3f6c872fd"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';