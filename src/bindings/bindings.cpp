//! @file bindings.cpp
//! Implements Pybind11 module xiangqi_bindings, exposing C++ classes and methods to
//! Python.

#include <pybind11/chrono.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <boardstate/zobrist.hpp>
#include <boardstate/zobrist_calculator_for_concepts.hpp>
#include <boardstate/zobrist_factories.hpp>
#include <boardstate/zobrist_for_concepts.hpp>
#include <concepts/board_state_coordinator.hpp>
#include <concepts/piece_value_provider.hpp>
#include <concepts/space_info_provider.hpp>
#include <game/game.hpp>
#include <game/game_data_structs.hpp>
#include <game/game_runner.hpp>
#include <gameboard/board_data_structs.hpp>
#include <gameboard/game_board.hpp>
#include <gameboard/game_board_for_concepts.hpp>
#include <moveselection/minimax_evaluator_factory.hpp>
#include <moveselection/move_evaluator_minimax_for_concepts.hpp>
#include <moveselection/move_evaluator_random_for_concepts.hpp>
#include <moveselection/move_evaluators.hpp>
#include <piecepoints/base_position_offset.hpp>
#include <piecepoints/piece_position_points.hpp>
#include <piecepoints/piece_position_points_for_concepts.hpp>
#include <random>
#include <string>
#include <utilities/integer_types.hpp>

namespace py = pybind11;
using namespace py::literals;
using namespace boardstate;
using namespace gameboard;
using namespace piecepoints;

template <typename KeyType, size_t NumConfKeys>
void bind_minimax_move_evaluator(py::module_ &m, const std::string &class_name) {
  py::class_<moveselection::MinimaxMoveEvaluator<
      GameBoard,
      ZobristCoordinator<KeyType, NumConfKeys>,
      PiecePositionPoints>>(m, class_name.c_str())
      .def(
          py::init<PieceColor, int, GameBoard &>(),
          "evaluating_player"_a,
          "search_depth"_a,
          "game_board"_a
      )
      .def(
          py::init<PieceColor, int, GameBoard &, uint32_t>(),
          "evaluating_player"_a,
          "search_depth"_a,
          "game_board"_a,
          "zkeys_seed"_a
      )
      .def(
          "select_move",
          &moveselection::MinimaxMoveEvaluator<
              GameBoard,
              ZobristCoordinator<KeyType, NumConfKeys>,
              PiecePositionPoints>::SelectMove,
          "allowed_moves"_a
      )
      .def_property_readonly(
          "search_summaries",
          &moveselection::MinimaxMoveEvaluator<
              GameBoard,
              ZobristCoordinator<KeyType, NumConfKeys>,
              PiecePositionPoints>::search_summaries
      )
      .def(
          "search_depth",
          &moveselection::MinimaxMoveEvaluator<
              GameBoard,
              ZobristCoordinator<KeyType, NumConfKeys>,
              PiecePositionPoints>::search_depth
      )
      .def(
          "zobrist_key_size_bits",
          &moveselection::MinimaxMoveEvaluator<
              GameBoard,
              ZobristCoordinator<KeyType, NumConfKeys>,
              PiecePositionPoints>::KeySizeBits
      )
      .def_property_readonly(
          "zkeys_seed",
          &moveselection::MinimaxMoveEvaluator<
              GameBoard,
              ZobristCoordinator<KeyType, NumConfKeys>,
              PiecePositionPoints>::zkeys_seed
      )
      .def_property_readonly(
          "board_state_hex_str",
          &moveselection::MinimaxMoveEvaluator<
              GameBoard,
              ZobristCoordinator<KeyType, NumConfKeys>,
              PiecePositionPoints>::board_state_hex_str
      );
}

PYBIND11_MODULE(xiangqi_bindings, m) {
  py::enum_<game::GameState>(m, "GameState")
      .value("kUnfinished", game::kUnfinished)
      .value("kDraw", game::kDraw)
      .value("kRedWon", game::kRedWon)
      .value("kBlkWon", game::kBlkWon);

  py::enum_<game::EvaluatorType>(m, "EvaluatorType")
      .value("kRandom", game::kRandom)
      .value("kMinimax", game::kMinimax)
      .value("kHuman", game::kHuman);

  py::class_<game::PlayerSpec>(m, "PlayerSpec")
      .def(
          py::init<PieceColor, game::EvaluatorType, size_t, size_t, DepthType, uint32_t>(
          ),
          py::arg("color"),
          py::arg("evaluator_type"),
          py::arg("zobrist_key_size_bits"),
          py::arg("zobrist_calculator_count"),
          py::arg("minimax_search_depth"),
          py::arg("zkeys_seed")
      )
      .def_readonly("color", &game::PlayerSpec::color)
      .def_readonly("evaluator_type", &game::PlayerSpec::evaluator_type)
      .def_readonly("zobrist_key_size_bits", &game::PlayerSpec::zobrist_key_size_bits)
      .def_readonly(
          "zobrist_calculator_count",
          &game::PlayerSpec::zobrist_calculator_count
      )
      .def_readonly("minimax_search_depth", &game::PlayerSpec::minimax_search_depth)
      .def_readonly("zkeys_seed", &game::PlayerSpec::zkeys_seed);

  py::class_<game::GameSummary>(m, "GameSummary")
      .def_readonly("game_id", &game::GameSummary::game_id)
      .def_readonly("game_state", &game::GameSummary::game_state)
      .def_readonly("move_log", &game::GameSummary::move_log)
      .def_readonly("player_specs", &game::GameSummary::player_specs)
      .def_readonly("search_summaries", &game::GameSummary::search_summaries);

  py::class_<game::GameRunner>(m, "GameRunner")
      .def(
          py::init<game::PlayerSpec, game::PlayerSpec>(),
          "red_player_spec"_a,
          "black_player_spec"_a
      )
      .def("run_game", &game::GameRunner::RunGame)
      .def_property_readonly("red_player_spec", &game::GameRunner::red_player_spec)
      .def_property_readonly("black_player_spec", &game::GameRunner::black_player_spec);

  // Define key types and their corresponding names as a tuple
  const std::tuple<std::string, size_t> key_types[] = {
      {"uint32_t", sizeof(uint32_t)},
      {"uint64_t", sizeof(uint64_t)},
      {"__uint128_t", sizeof(__uint128_t)}
  };

  py::class_<PiecePositionPoints>(m, "PiecePositionPoints").def(py::init<>());

  py::class_<moveselection::PreSearchMoveSorter<GameBoard, PiecePositionPoints>>(
      m,
      "PreSearchMoveSorter"
  )
      .def(
          py::init<GameBoard &, PiecePositionPoints &>(),
          "game_board"_a,
          "game_position_points"_a
      );

  py::class_<BoardSpace>(m, "BoardSpace")
      .def(py::init<int, int>(), "rank"_a, "file"_a)
      .def_readonly("rank", &BoardSpace::rank)
      .def_readonly("file", &BoardSpace::file);

  py::class_<GamePiece>(m, "GamePiece")
      .def(py::init<>())
      .def(py::init<int>(), "int_piece"_a)
      .def(py::init<PieceType, PieceColor>(), "piece_type"_a, "piece_color"_a)
      .def_readonly("piece_type", &GamePiece::piece_type)
      .def_readonly("piece_color", &GamePiece::piece_color);

  py::class_<Move>(m, "Move")
      .def(py::init<BoardSpace, BoardSpace>(), "start"_a, "end"_a)
      .def_readonly("start", &Move::start)
      .def_readonly("end", &Move::end);

  py::class_<MoveCollection>(m, "MoveCollection")
      .def_readonly("moves", &MoveCollection::moves)
      .def("size", &MoveCollection::Size)
      .def("ContainsMove", &MoveCollection::ContainsMove);

  py::class_<moveselection::EqualScoreMoves>(m, "EqualScoreMoves")
      .def_readonly("shared_score", &moveselection::EqualScoreMoves::shared_score)
      .def_property_readonly(
          "move_collection",
          &moveselection::EqualScoreMoves::move_collection
      );

  py::class_<ExecutedMove>(m, "ExecutedMove")
      .def(
          py::init<Move, int, int>(),
          "spaces"_a,
          "moving_piece"_a,
          "destination_piece"_a
      )
      .def_readonly("spaces", &ExecutedMove::spaces)
      .def_readonly("moving_piece", &ExecutedMove::moving_piece)
      .def_readonly("destination_piece", &ExecutedMove::destination_piece);

  py::enum_<PieceColor>(m, "PieceColor")
      .value("kRed", kRed)
      .value("kNul", kNul)
      .value("kBlk", kBlk)
      .export_values();

  py::enum_<PieceType>(m, "PieceType")
      .value("kNnn", kNnn)
      .value("kGen", kGen)
      .value("kAdv", kAdv)
      .value("kEle", kEle)
      .value("kHor", kHor)
      .value("kCha", kCha)
      .value("kCan", kCan)
      .value("kSol", kSol)
      .export_values();

  m.def("size_of_points_type", &size_of_points_type);
  m.def("is_signed_points_type", &is_signed_points_type);

  py::enum_<moveselection::MinimaxResultType>(m, "MinimaxResultType")
      .value("Unknown", moveselection::kUnknown)
      .value("TrTableHit", moveselection::kTrTableHit)
      //   .value("TrTableHitEvaluatorLoses", kTrTableHitEvaluatorLoses)
      //   .value("TrTableHitEvaluatorWins", kTrTableHitEvaluatorWins)
      .value("EvaluatorLoses", moveselection::kEvaluatorLoses)
      .value("EvaluatorWins", moveselection::kEvaluatorWins)
      .value("Draw", moveselection::kDraw)
      .value("FullyEvaluatedNode", moveselection::kFullyEvaluatedNode)
      .value("StandardLeaf", moveselection::kStandardLeaf)
      .value("AlphaPrune", moveselection::kAlphaPrune)
      .value("BetaPrune", moveselection::kBetaPrune)
      .export_values();

  py::class_<GameBoard>(m, "GameBoard")
      .def(py::init<>())
      .def("map", &GameBoard::map)
      .def("ExecuteMove", &GameBoard::ExecuteMove, "move"_a)
      .def("UndoMove", &GameBoard::UndoMove, "executed_move"_a)
      .def("GetAllSpacesOccupiedBy", &GameBoard::GetAllSpacesOccupiedBy, "color"_a)
      .def("CalcFinalMovesOf", &GameBoard::CalcFinalMovesOf, "color"_a)
      .def("IsInCheck", &GameBoard::IsInCheck, "color"_a)
      .def("GetType", &GameBoard::GetType, "space"_a)
      .def_property_readonly("move_log", &GameBoard::move_log)
      .def_property_readonly("is_draw", &GameBoard::IsDraw)
      .def("GetColor", &GameBoard::GetColor, "space"_a);

  m.def("opponent_of", &opponent_of);

  py::class_<moveselection::RandomMoveEvaluator<GameBoard>>(m, "RandomMoveEvaluator")
      .def(py::init<PieceColor, GameBoard &>(), "evaluating_player"_a, "game_board"_a)
      .def(
          "select_move",
          &moveselection::RandomMoveEvaluator<GameBoard>::SelectMove,
          "allowed_moves"_a
      );

  py::class_<moveselection::SearchSummary>(m, "SearchSummary")
      .def_property_readonly(
          "num_nodes",
          &moveselection::SearchSummary::num_nodes
      ) // Read-only access to fields
      .def_property_readonly("time", &moveselection::SearchSummary::time)
      .def(
          "get_result_depth_counts",
          &moveselection::SearchSummary::GetResultDepthCounts
      )
      .def(
          "get_transposition_table_hits",
          &moveselection::SearchSummary::GetTranspositionTableHits
      )
      .def_property_readonly(
          "equal_score_moves",
          &moveselection::SearchSummary::equal_score_moves
      )
      .def_property_readonly(
          "selected_move",
          &moveselection::SearchSummary::selected_move
      )
      .def_property_readonly(
          "returned_illegal_move",
          &moveselection::SearchSummary::returned_illegal_move
      )
      .def_property_readonly(
          "num_collisions",
          &moveselection::SearchSummary::num_collisions
      )
      .def_property_readonly(
          "tr_table_size_initial",
          &moveselection::SearchSummary::tr_table_size_initial
      )
      .def_property_readonly(
          "tr_table_size_final",
          &moveselection::SearchSummary::tr_table_size_final
      );

  py::class_<moveselection::SearchSummaries>(m, "SearchSummaries")
      .def(py::init<>()) // Constructor, as needed for initialization
      .def_readonly(
          "first_searches",
          &moveselection::SearchSummaries::first_searches
      ) // Read-only vectors and maps
      .def_readonly("extra_searches", &moveselection::SearchSummaries::extra_searches);

  py::class_<TranspositionTableGuard>(m, "TranspositionTableGuard").def(py::init<>());

  //   Evaluators without dependency injection (except GameBoard)
  bind_minimax_move_evaluator<uint32_t, 0>(m, "MinimaxMoveEvaluator32");
  bind_minimax_move_evaluator<uint64_t, 0>(m, "MinimaxMoveEvaluator64");
  bind_minimax_move_evaluator<__uint128_t, 0>(m, "MinimaxMoveEvaluator128");
  bind_minimax_move_evaluator<uint32_t, 1>(m, "MinimaxMoveEvaluator32Dual");
  bind_minimax_move_evaluator<uint64_t, 1>(m, "MinimaxMoveEvaluator64Dual");
  bind_minimax_move_evaluator<__uint128_t, 1>(m, "MinimaxMoveEvaluator128Dual");
}