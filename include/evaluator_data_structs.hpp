//! @file evaluator_data_structs.hpp
//! Data structs used by moveselection::MinimaxEvaluator

#pragma once

#include <board_data_structs.hpp>
#include <chrono>
#include <map>
#include <move_data_structs.hpp>
#include <piece_points_bpo.hpp>

using namespace gameboard;
using namespace piecepoints;

namespace moveselection {

//! Holds a gameboard::MoveCollection in which all gameboard::Move have the same value
//! (as perceived by a MoveEvaluator), and the value of the shared score.
struct EqualScoreMoves {
  Points_t shared_score;
  MoveCollection similar_moves;
};

//! A gameboard::Move, and an associated score calculated by a MoveEvaluator.
struct ScoredMove {
  Move move;
  Points_t score;
};

enum MinimaxResultType : size_t {
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
const size_t kNumResultTypes{7};

//! Data structure that holds a moveselection::EqualScoreMoves and other search-related
//! info obtained from a call to moveselection::MinimaxMoveEvaluator.MinimaxRec.
struct TranspositionTableEntry {
  TranspositionTableEntry()
      : remaining_search_depth{}
      , result_type{}
      , similar_moves{} {}

  TranspositionTableEntry(int depth, MinimaxResultType type, EqualScoreMoves moves)
      : remaining_search_depth(depth)
      , result_type(type)
      , similar_moves(std::move(moves)) {}

  int remaining_search_depth;
  MinimaxResultType result_type;
  EqualScoreMoves similar_moves;

  Points_t Score() { return similar_moves.shared_score; }
};

//! Container for storing a moveselection::TranspositionTableEntry retrieved by a call to
//! boardstate::SingleZobristTracker.ImplementGetTrData.
struct TranspositionTableSearchResult {
  TranspositionTableEntry table_entry;
  bool found;
  bool known_collision;
};

struct TranspositionTableSize {
  uint64_t num_entries;
  uint64_t num_states;
};

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
  ResultDepthCounts(int max_search_depth) {
    for (auto &vec : data_) {
      vec.resize(max_search_depth + 1);
    }
  }

  void Update(MinimaxResultType result_type, int search_depth) {
    data_[result_type][search_depth]++;
  }

  ResultDepthCountsData_t data() { return data_; }

private:
  ResultDepthCountsData_t data_;
};

//! Stores data collected during a single call to
//! moveselection::MinimaxMoveEvaluator.ImplementSelectMove. Includes total number of
//! nodes explored, a moveseelection::ResultDepthCounts object for
//! moveselection::MinimaxResultType::kTrTableHit search results, and another
//! moveseelection::ResultDepthCounts  for all other search result types.
class SearchSummary {
public:
  SearchSummary(int max_search_depth, TranspositionTableSize tr_table_size_initial)
      : num_nodes_{}
      , result_depth_counts_{ResultDepthCounts(max_search_depth)}
      , transposition_table_hits_(ResultDepthCounts(max_search_depth))
      , returned_illegal_move_{false}
      , num_collisions_{0}
      , tr_table_size_initial_{tr_table_size_initial}
      , tr_table_size_final_{} {}

  void Update(
      MinimaxResultType result_type,
      int search_depth,
      EqualScoreMoves similar_moves
  ) {
    // result_depth_counts[result_type][search_depth]++;
    result_depth_counts_.Update(result_type, search_depth);
    num_nodes_++;
    set_similar_moves(similar_moves);
  }

  void UpdateTranspositionTableHits(MinimaxResultType result_type, int search_depth) {
    transposition_table_hits_.Update(result_type, search_depth);
  }

  void RecordTrTableHitInfo(
      MinimaxResultType result_type,
      int remaining_search_depth,
      TranspositionTableSearchResult tr_table_search_result
  ) {
    Update(
        result_type,
        remaining_search_depth,
        tr_table_search_result.table_entry.similar_moves
    );
    UpdateTranspositionTableHits(result_type, remaining_search_depth);
    if (tr_table_search_result.known_collision) {
      num_collisions_++;
    }
  }

  int num_nodes() { return num_nodes_; }
  std::chrono::duration<double, std::nano> time() { return time_; }

  void set_time(std::chrono::duration<double, std::nano> search_time) {
    time_ = search_time;
  }
  EqualScoreMoves similar_moves() { return similar_moves_; }
  void set_similar_moves(EqualScoreMoves similar_moves) {
    similar_moves_ = similar_moves;
  }

  Move selected_move() { return selected_move_; }
  void SetSelectedMove(Move selected_move) { selected_move_ = selected_move; }
  ResultDepthCountsData_t GetResultDepthCounts() { return result_depth_counts_.data(); }
  ResultDepthCountsData_t GetTranspositionTableHits() {
    return transposition_table_hits_.data();
  }
  TranspositionTableSize tr_table_size_initial() { return tr_table_size_initial_; }
  TranspositionTableSize tr_table_size_final() { return tr_table_size_final_; }
  void set_tr_table_size_final(TranspositionTableSize tr_table_size_final) {
    tr_table_size_final_ = tr_table_size_final;
  }
  void set_returned_illegal_move(bool status) { returned_illegal_move_ = status; }
  bool returned_illegal_move() { return returned_illegal_move_; }
  int num_collisions() {return num_collisions_; }

private:
  int num_nodes_;
  std::chrono::duration<double, std::nano> time_;
  ResultDepthCounts result_depth_counts_;
  ResultDepthCounts transposition_table_hits_;
  EqualScoreMoves similar_moves_;
  Move selected_move_;
  bool returned_illegal_move_;
  int num_collisions_;
  TranspositionTableSize tr_table_size_initial_;
  TranspositionTableSize tr_table_size_final_;
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
  std::map<int, SearchSummary> extra_searches;

  SearchSummary &NewFirstSearch(
      int search_depth,
      TranspositionTableSize tr_table_size_initial
  ) {
    first_searches.emplace_back(SearchSummary(search_depth, tr_table_size_initial));
    return first_searches.back();
  }

  SearchSummary &NewExtraSearch(
      int search_depth,
      int search_number,
      TranspositionTableSize tr_table_size_current
  ) {
    auto new_search_entry = extra_searches.emplace(
        search_number,
        SearchSummary(search_depth, tr_table_size_current)
    );
    return new_search_entry.first->second;
  }
};
} // namespace moveselection
