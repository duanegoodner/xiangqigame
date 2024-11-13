#include <iostream>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>

class TranspositionTableSearchResult {
private:
  bool found_;
  uint32_t value_;

public:
  bool found() { return found_; }
  void set_found(bool status) { found_ = status; }

  uint32_t value() { return value_; }
  void set_value(uint32_t value) { value_ = value; }
};

// Resource
class TranspositionTable {
private:
  std::unordered_map<uint32_t, uint32_t> data_;

public:
  TranspositionTable() {}

  void GetDataAt(uint32_t key, TranspositionTableSearchResult &result) {
    GetDataAtInternal(key, result);
  }

  void WriteData(uint32_t key, uint32_t value) { data_.insert_or_assign(key, value); }

  size_t size() { return data_.size(); }

private:
 void GetDataAtInternal(uint32_t key, TranspositionTableSearchResult &result) {
    auto entry_it = data_.find(key);
    if (entry_it != data_.end()) {
      result.set_found(true);
      result.set_value(entry_it->second);
    }
 }
};

class TranspositionTableGuard {
    

};



// Child
class ZobristManager {
private:
  TranspositionTable transposition_table_;
  mutable std::shared_mutex mutex_;


public:
  ZobristManager() {}


  template <typename TrTableOp>
  auto WithLockedTrTable(TrTableOp &&operation) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    return operation(transposition_table_);
  }
};

// Worker???
// class Pruner {
//     private:
//     TranspositionTable& transposition_table_;
//     size_t move_counts_;


// };


// Parent
class MinimaxEvaluator {
private:
  std::string name_;
  ZobristManager zobrist_tracker_;
  std::mt19937 rng_;
  size_t num_nodes_per_cycle_;

public:
  explicit MinimaxEvaluator(
      std::string name,
      size_t num_nodes_per_cycle,
      uint32_t rng_seed = std::random_device{}()
  )
      : name_{name}
      , num_nodes_per_cycle_{num_nodes_per_cycle}
      , rng_{std::random_device{}()} {}

  void EvaluateNodes() {
    zobrist_tracker_.WithLockedTrTable([this](TranspositionTable &tr_table) {
      EvaluateMultipleNodesInternal(tr_table);
    });
  }

  void GetTrTableSize(size_t &result) {
    zobrist_tracker_.WithLockedTrTable([this, &result](TranspositionTable &tr_table) {
      result = GetTrTableSizeInternal(tr_table);
    });
  }

private:
  uint32_t GetBoardState() { return (uint32_t)rng_() % 200; }
  uint32_t GetCalculationResult() { return (uint32_t)rng_() % 200; }

  TranspositionTableSearchResult GetTrTableSearchResultAt(
      uint32_t board_state,
      TranspositionTable &tr_table
  ) {
    TranspositionTableSearchResult search_result{};
    tr_table.GetDataAt(board_state, search_result);

    return search_result;
  }

  void DisplayKeepExistingValueMessage(
      uint32_t board_state,
      uint32_t existing_value,
      uint32_t current_result
  ) {
    std::cout << name_ << " kept (" << board_state << ", " << existing_value
              << " instead of writing (" << board_state << ", " << current_result << ")"
              << std::endl;
  }

  void DisplayWriteNewValueMessage(uint32_t board_state, uint32_t current_result) {
    std::cout << name_ << " added (" << board_state << ", " << current_result << ")"
              << std::endl;
  }

  void UpdateTrTableWith(
      uint32_t board_state,
      uint32_t current_result,
      TranspositionTableSearchResult &search_result,
      TranspositionTable &tr_table
  ) {
    if (search_result.found()) {
      if (current_result <= search_result.value()) {
        DisplayKeepExistingValueMessage(
            board_state,
            search_result.value(),
            current_result
        );
        return;
      }
    }
    tr_table.WriteData(board_state, current_result);
    DisplayWriteNewValueMessage(board_state, current_result);
  }

  void EvaluateNodeInternal(TranspositionTable &tr_table) {
    std::this_thread::sleep_for(std::chrono::microseconds(25));
    auto board_state = (uint32_t)rng_() % 200;
    auto current_result = (uint32_t)rng_() % 42;
    auto search_result = GetTrTableSearchResultAt(board_state, tr_table);
    UpdateTrTableWith(board_state, current_result, search_result, tr_table);
  }

  void EvaluateMultipleNodesInternal(TranspositionTable &tr_table) {
    std::cout << name_ << " beginning to evaluate nodes -------" << std::endl;
    for (auto i = 0; i < num_nodes_per_cycle_; ++i) {
      EvaluateNodeInternal(tr_table);
    }
    std::cout << name_ << " done evaluating nodes -------" << std::endl;
    std::cout << std::endl;
  }

  size_t GetTrTableSizeInternal(TranspositionTable &tr_table) { return tr_table.size(); }
};

class GameManager {
private:
  MinimaxEvaluator &evaluator1_;
  MinimaxEvaluator &evaluator2_;

public:
  GameManager(MinimaxEvaluator &e1, MinimaxEvaluator &e2)
      : evaluator1_{e1}
      , evaluator2_{e2} {}

  void RunGame() {
    size_t turn_number = 0;
    size_t tr_table_size1{0};
    size_t tr_table_size2{0};
    while (tr_table_size1 <= 15 && tr_table_size2 <= 15) {
      if (turn_number % 2 == 0) {
        evaluator1_.EvaluateNodes();
        evaluator1_.GetTrTableSize(tr_table_size1);
      } else {
        evaluator2_.EvaluateNodes();
        evaluator2_.GetTrTableSize(tr_table_size2);
      }
      turn_number++;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }
};

int main() {
  ZobristManager zobrist_tracker;
  MinimaxEvaluator black_team_evaluator{"black", 5};
  MinimaxEvaluator red_team_evaluator{"red", 5};
  GameManager game_manager{black_team_evaluator, red_team_evaluator};

  game_manager.RunGame();

  return 0;
}
