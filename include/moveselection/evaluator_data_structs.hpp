//! @file moveselection/evaluator_data_structs.hpp
//! Data structs used by moveselection::MinimaxEvaluator

#pragma once

#include <gameboard/board_data_structs.hpp>
// #include <interfaces/board_state_summarizer_interface.hpp>
#include <chrono>
#include <utilities/integer_types.hpp>
#include <map>
#include <gameboard/move_data_structs.hpp>
#include <piecepoints/base_position_offset.hpp>

using namespace gameboard;
using namespace piecepoints;

namespace moveselection {


//! Holds a gameboard::MoveCollection in which all gameboard::Move have the same value
//! (as perceived by a MoveEvaluator), and the value of the shared score.
class EqualScoreMoves {
public:
  Points_t shared_score;
  MoveCollection move_collection_;

  MoveCollection move_collection() { return move_collection_; }
};

//! A gameboard::Move, and an associated score calculated by a MoveEvaluator.
struct ScoredMove {
  Move move;
  Points_t score;
};

enum MinimaxResultType : uint16_t {
  kUnknown = 0,
  kTrTableHit = 1,
  kEvaluatorLoses = 4,
  kEvaluatorWins = 5,
  kDraw = 6,
  kFullyEvaluatedNode = 7,
  kStandardLeaf = 8,
  kAlphaPrune = 9,
  kBetaPrune = 10,
  kMin = kUnknown,
  kMax = kBetaPrune
};
const uint16_t kNumResultTypes{7};

//! Data structure that holds a moveselection::EqualScoreMoves and other search-related
//! info obtained from a call to moveselection::MinimaxMoveEvaluator.MinimaxRecursive.
class MinimaxCalcResult {
public:
  MinimaxCalcResult()
      : remaining_search_depth_{}
      , result_type_{}
      , equal_score_moves_{} {}

  MinimaxCalcResult(DepthType depth, MinimaxResultType type, EqualScoreMoves moves)
      : remaining_search_depth_{depth}
      , result_type_{type}
      , equal_score_moves_{std::move(moves)} {}

  Points_t Score() { return equal_score_moves_.shared_score; }
  EqualScoreMoves equal_score_moves() { return equal_score_moves_; }
  MoveCollection moves() { return equal_score_moves_.move_collection(); }
  MinimaxResultType result_type() { return result_type_; }
  DepthType remaining_search_depth() { return remaining_search_depth_; }

private:
  DepthType remaining_search_depth_;
  MinimaxResultType result_type_;
  EqualScoreMoves equal_score_moves_;
};

//! Container for storing a moveselection::MinimaxCalcResult retrieved by a call to
//! boardstate::SingleZobristCoordinator.ImplementGetTrData.
class TranspositionTableSearchResult {
public:
  MoveCollection moves() { return minimax_calc_result_.moves(); }

  bool IsConsistentWith(const MoveCollection &allowed_moves) {
    if (allowed_moves.IsEmpty() and moves().IsEmpty()) {
      return false;
    }
    if (allowed_moves.IsEmpty() and moves().IsEmpty()) {
      return true;
    }
    if (moves().ContainsAnyMoveNotIn(allowed_moves)) {
      return false;
    }
    return true;
  }

  bool found() { return found_; }
  void set_found(bool status) { found_ = status; }

  bool known_collision() { return known_collision_; }
  void set_known_collision(bool status) { known_collision_ = status; }

  MinimaxCalcResult minimax_calc_result() { return minimax_calc_result_; }
  void set_minimax_calc_result(MinimaxCalcResult result) {
    minimax_calc_result_ = result;
  }

  EqualScoreMoves score_and_moves() { return minimax_calc_result_.equal_score_moves(); }

  MinimaxResultType result_type() { return minimax_calc_result_.result_type();}

private:
  MinimaxCalcResult minimax_calc_result_;
  bool found_;
  bool known_collision_;
};

// struct TranspositionTableSize {
//   uint64_t num_entries;
//   uint64_t num_states;
// };

//! Array of vectors for storing counts of moveselection::MinimaxResultType for each
//! posible remaining search depth. Outer (array) index ->
//! moveslection::MinimaxResultType; inner (vector) index -> remaining search depth when
//! result was obtained.
typedef std::array<std::vector<int>, MinimaxResultType::kMax + 1>
    ResultDepthCountsData_t;

//! Container for storing and updating data in a moveselection::ResultDepthCountsData_t
//! array of vectors.
class ResultDepthCounts {
public:
  ResultDepthCounts(DepthType max_search_depth) {
    for (auto &vec : data_) {
      vec.resize(max_search_depth + 1);
    }
  }

  void IncrementDataAt(MinimaxResultType result_type, DepthType search_depth) {
    data_[result_type][search_depth]++;
  }

  ResultDepthCountsData_t data() { return data_; }

private:
  ResultDepthCountsData_t data_;
};

// struct CollisionInfo {
//   TranspositionTableSize tr_table_size;
//   std::string board_state;
// };

//! Stores data collected during a single call to
//! moveselection::MinimaxMoveEvaluator.ImplementSelectMove. Includes total number of
//! nodes explored, a moveseelection::ResultDepthCounts object for
//! moveselection::MinimaxResultType::kTrTableHit search results, and another
//! moveseelection::ResultDepthCounts  for all other search result types.
class SearchSummary {
public:
  SearchSummary(DepthType max_search_depth, size_t tr_table_size_initial)
      : num_nodes_{}
      , result_depth_counts_{ResultDepthCounts(max_search_depth)}
      , transposition_table_hits_(ResultDepthCounts(max_search_depth))
      , returned_illegal_move_{false}
      , num_collisions_{0}
      , tr_table_size_initial_{tr_table_size_initial}
      , tr_table_size_final_{} {}

  void RecordNodeInfo(
      MinimaxResultType result_type,
      DepthType search_depth,
      const EqualScoreMoves &equal_score_moves
  ) {
    result_depth_counts_.IncrementDataAt(result_type, search_depth);
    num_nodes_++;
    set_equal_score_moves(equal_score_moves);
  }

  void UpdateTranspositionTableHits(MinimaxResultType result_type, DepthType search_depth) {
    transposition_table_hits_.IncrementDataAt(result_type, search_depth);
  }

  void RecordTrTableHit(
      TranspositionTableSearchResult &tr_table_search_result,
      DepthType remaining_search_depth
  ) {
    RecordNodeInfo(
        MinimaxResultType::kTrTableHit,
        remaining_search_depth,
        tr_table_search_result.score_and_moves()
    );
    UpdateTranspositionTableHits(
        tr_table_search_result.result_type(),
        remaining_search_depth
    );
    if (tr_table_search_result.known_collision()) {
      num_collisions_++;
    }
  }

  size_t num_nodes() { return num_nodes_; }
  std::chrono::duration<double, std::nano> time() { return time_; }

  void set_time(std::chrono::duration<double, std::nano> search_time) {
    time_ = search_time;
  }
  EqualScoreMoves equal_score_moves() { return equal_score_moves_; }
  void set_equal_score_moves(EqualScoreMoves equal_score_moves) {
    equal_score_moves_ = equal_score_moves;
  }

  Move selected_move() { return selected_move_; }
  void SetSelectedMove(Move selected_move) { selected_move_ = selected_move; }
  ResultDepthCountsData_t GetResultDepthCounts() { return result_depth_counts_.data(); }
  ResultDepthCountsData_t GetTranspositionTableHits() {
    return transposition_table_hits_.data();
  }
  size_t tr_table_size_initial() { return tr_table_size_initial_; }
  size_t tr_table_size_final() { return tr_table_size_final_; }
  void set_tr_table_size_final(size_t tr_table_size_final) {
    tr_table_size_final_ = tr_table_size_final;
  }
  void set_returned_illegal_move(bool status) { returned_illegal_move_ = status; }
  bool returned_illegal_move() { return returned_illegal_move_; }
  size_t num_collisions() { return num_collisions_; }

private:
  size_t num_nodes_;
  std::chrono::duration<double, std::nano> time_;
  ResultDepthCounts result_depth_counts_;
  ResultDepthCounts transposition_table_hits_;
  EqualScoreMoves equal_score_moves_;
  Move selected_move_;
  bool returned_illegal_move_;
  size_t num_collisions_;
  size_t tr_table_size_initial_;
  size_t tr_table_size_final_;
};

//! Stores a moveselection::SearchSummary for each
//! moveselection::MinimaxMoveEvaluator.ImplementSelectMove made for a particular player
//! during a game.
//! If a second call to moveselection::MinimaxMoveEvaluator.RunMinimax is needed (due to
//! a hash collision causing in illegal move to be returned by the first call to to
//! moveselection::MinimaxMoveEvaluator.RunMinimax), a moveselection::SearchSummary for
//! the second search is stored in moveselection::SearchSummaries.extra_searches.
struct SearchSummaries {
  std::vector<SearchSummary> first_searches;
  std::map<MoveCountType, SearchSummary> extra_searches;

  SearchSummary &NewFirstSearch(DepthType search_depth, size_t tr_table_size_initial) {
    first_searches.emplace_back(SearchSummary(search_depth, tr_table_size_initial));
    return first_searches.back();
  }

  SearchSummary &NewExtraSearch(
      DepthType search_depth,
      MoveCountType search_number,
      size_t tr_table_size_current
  ) {
    auto new_search_entry = extra_searches.emplace(
        search_number,
        SearchSummary(search_depth, tr_table_size_current)
    );
    return new_search_entry.first->second;
  }
};


} // namespace moveselection
