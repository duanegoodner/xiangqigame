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
      [ "Engine Data Collection", "index.html#autotoc_md4", null ]
    ] ],
    [ "Installing", "md_docs_02_installing.html", [
      [ "Native Installation", "md_docs_02_installing.html#autotoc_md6", [
        [ "Requirements", "md_docs_02_installing.html#autotoc_md7", null ],
        [ "Procedure", "md_docs_02_installing.html#autotoc_md8", null ],
        [ "Native Installation Demo", "md_docs_02_installing.html#autotoc_md9", null ]
      ] ],
      [ "Docker Installation", "md_docs_02_installing.html#autotoc_md10", [
        [ "Requirements", "md_docs_02_installing.html#autotoc_md11", null ],
        [ "Procedure", "md_docs_02_installing.html#autotoc_md12", null ],
        [ "Stopping the Docker Container", "md_docs_02_installing.html#autotoc_md13", null ],
        [ "Re-Starting the Docker Container", "md_docs_02_installing.html#autotoc_md14", null ],
        [ "Demo", "md_docs_02_installing.html#autotoc_md15", null ]
      ] ]
    ] ],
    [ "Running", "md_docs_03_running.html", [
      [ "The play_xiangqi Command", "md_docs_03_running.html#autotoc_md17", null ],
      [ "Example Games", "md_docs_03_running.html#autotoc_md18", [
        [ "Game #1:", "md_docs_03_running.html#autotoc_md19", [
          [ "Settings", "md_docs_03_running.html#autotoc_md20", null ],
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md21", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md22", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md23", null ]
        ] ],
        [ "Red=SD5, Black=SD3", "md_docs_03_running.html#autotoc_md24", [
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md25", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md26", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md27", null ]
        ] ],
        [ "Red=SD3, Black=SD5", "md_docs_03_running.html#autotoc_md28", [
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md29", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md30", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md31", null ]
        ] ],
        [ "Red=Random, Black=SD5", "md_docs_03_running.html#autotoc_md32", [
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md33", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md34", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md35", null ]
        ] ],
        [ "Red=SD6, Black=SD6", "md_docs_03_running.html#autotoc_md36", [
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md37", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md38", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md39", null ]
        ] ],
        [ "Red=SD5:K64, Black=SD5:K128", "md_docs_03_running.html#autotoc_md40", [
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md41", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md42", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md43", null ]
        ] ],
        [ "Red=Human, Black=SD4", "md_docs_03_running.html#autotoc_md44", [
          [ "Terminal Recording", "md_docs_03_running.html#autotoc_md45", null ],
          [ "Minimax Data", "md_docs_03_running.html#autotoc_md46", null ],
          [ "Synopsis", "md_docs_03_running.html#autotoc_md47", null ]
        ] ]
      ] ]
    ] ],
    [ "Terminal Game Board and Notation", "md_docs_04_notation.html", [
      [ "Piece Notation", "md_docs_04_notation.html#autotoc_md49", [
        [ "Board Space Notation", "md_docs_04_notation.html#autotoc_md52", null ],
        [ "Move Notation", "md_docs_04_notation.html#autotoc_md53", null ],
        [ "Board Features", "md_docs_04_notation.html#autotoc_md54", [
          [ "First character (piece type)", "md_docs_04_notation.html#autotoc_md50", null ],
          [ "Second character (piece color)", "md_docs_04_notation.html#autotoc_md51", null ],
          [ "River Edges", "md_docs_04_notation.html#autotoc_md55", null ],
          [ "Castle Spaces", "md_docs_04_notation.html#autotoc_md56", null ]
        ] ]
      ] ]
    ] ],
    [ "AI Engine Data", "md_docs_05_game_data.html", null ],
    [ "Components", "md_docs_06_components.html", [
      [ "UML Diagram", "md_docs_06_components.html#autotoc_md59", null ],
      [ "Component Descriptions", "md_docs_06_components.html#autotoc_md60", [
        [ "jsonio + JsonInterface", "md_docs_06_components.html#autotoc_md61", null ],
        [ "piecepoints + PieceValueProvider", "md_docs_06_components.html#autotoc_md62", null ],
        [ "gameboard + SpaceInfoProvider", "md_docs_06_components.html#autotoc_md63", null ],
        [ "hashcalculator + BoardStateCoordinator", "md_docs_06_components.html#autotoc_md64", null ],
        [ "MoveEvaluator (moveselection)", "md_docs_06_components.html#autotoc_md65", null ]
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
"classboardstate_1_1TranspositionTableEntry.html#a3b690dae3e246f3869cba774387f4190",
"classgameboard_1_1PieceMoves.html#a863d33ef5eeeb1be6a4d2e10013bdff3",
"classmoveselection_1_1TranspositionTableSearchResult.html#a7b48f74b7b3c1650f8abbd3fa11e3f1c",
"classxiangqipy_1_1core__dataclass__mirrors_1_1TranspositionTableSizesAtEvents.html#aeb23a70f654186d2bf93b92ea179536a",
"classxiangqipy_1_1game__summary__plotters_1_1SearchTimePlotter.html",
"dir_ea91a2a72b7cbbf988d2bd2cd1d23e49.html",
"md_docs_03_running.html#autotoc_md27",
"namespacexiangqipy_1_1player__builder.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';