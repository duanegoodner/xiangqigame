#include <iostream>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <thread>

bool RandomTrueWithProbability(double probability) {
  // Create a random number generator
  std::random_device rd;
  std::mt19937 gen(rd());

  // Define a distribution for a double range between 0.0 and 1.0
  std::uniform_real_distribution<> distrib(0.0, 1.0);

  // Generate a random number and return true if it's less than or equal to the specified
  // probability
  return distrib(gen) <= probability;
}

class TranspositionTable {
  std::string player_name_;

public:
  TranspositionTable(std::string player_name)
      : player_name_{player_name} {}

  void ReadWrite() {
    Read();
    Write();
    std::cout << std::endl << std::flush;
  }

  void ReadRemove() {
    Read();
    RemoveData();
    std::cout << std::endl << std::flush;
  }

  void Read() const {
    std::cout << player_name_ << " ooo READING ooo" << std::endl << std::flush;
  }

  void Write() {
    std::cout << player_name_ << " --- WRITING ---" << std::endl << std::flush;
  }

  void RemoveData() {
    std::cout << player_name_ << " xxx DELETING xxx"
              << std::endl << std::flush;
  }
};

class TranspositionTableGuard {
  mutable std::mutex tr_table_mutex_;

public:
  std::unique_lock<std::mutex> GetExclusiveLock() {
    return std::unique_lock<std::mutex>(tr_table_mutex_);
  }
};

class TranspositionTablePruner {
  std::string player_name_;
  TranspositionTable &tr_table_;
  TranspositionTableGuard &tr_table_guard_;
  std::thread pruning_thread_;
  std::atomic<bool> keep_running_;

public:
  TranspositionTablePruner(
      std::string player_name,
      TranspositionTable &tr_table,
      TranspositionTableGuard &tr_table_guard
  )
      : player_name_{player_name}
      , tr_table_{tr_table}
      , tr_table_guard_{tr_table_guard}
      , pruning_thread_{}
      , keep_running_{true} {}

  ~TranspositionTablePruner() {
    if (pruning_thread_.joinable()) {
      pruning_thread_.join();
    }
  }

  void ThreadSafePrune() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto lock = tr_table_guard_.GetExclusiveLock();
    UnsafePrune();
  }

  void ContinuouslyPrune() {
    while (keep_running_) {
      ThreadSafePrune();
    }
  }

  void Stop() { keep_running_ = false; }

  void Run() {
    std::cout << player_name_ << " PRUNER starting" << "\n" << std::endl << std::flush;
    pruning_thread_ = std::thread(&TranspositionTablePruner::ContinuouslyPrune, this);
  }

private:
  void UnsafePrune() {
    tr_table_.ReadRemove();
  }
};

class ZobristManager {
  std::string player_name_;
  TranspositionTableGuard tr_table_guard_;
  TranspositionTable tr_table_;
  TranspositionTablePruner tr_table_pruner_;

public:
  ZobristManager(std::string player_name)
      : player_name_{player_name}
      , tr_table_guard_{}
      , tr_table_{player_name_}
      , tr_table_pruner_{
            TranspositionTablePruner{player_name_, tr_table_, tr_table_guard_}
        } {}

  auto LockTranspositionTable() { return tr_table_guard_.GetExclusiveLock(); }

  void ReadWriteTrTable() { tr_table_.ReadWrite(); }
  void StartPruner() { tr_table_pruner_.Run(); }
  void StopPruner() { tr_table_pruner_.Stop(); }
};

class MinimaxMoveEvaluator {
  std::string player_name_;
  ZobristManager zobrist_manager_;

public:
  MinimaxMoveEvaluator(std::string player_name)
      : player_name_{player_name}
      , zobrist_manager_{ZobristManager{player_name}} {}

  std::string player_name() { return player_name_; }

  void SelectMove() {
    auto lock = zobrist_manager_.LockTranspositionTable();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    UnsafeSelectMove();
  }

  void Start() { zobrist_manager_.StartPruner(); }
  void Stop() { zobrist_manager_.StopPruner(); }

private:
  void UnsafeSelectMove() { zobrist_manager_.ReadWriteTrTable(); }
};

class GameManager {
  MinimaxMoveEvaluator evaluator_red_;
  MinimaxMoveEvaluator evaluator_black_;
  double game_over_probability_;
  size_t turn_number_;
  std::string winner_;

public:
  GameManager(double game_over_probability = 0.1)
      : evaluator_red_{"RED"}
      , evaluator_black_{"BLACK"}
      , game_over_probability_{game_over_probability}
      , turn_number_{0}
      , winner_{} {}

  void RunGame() { RunGameInternal(); }

private:
  bool IsGameOver() { return RandomTrueWithProbability(game_over_probability_); }

  void IncrementTurnNumber() { turn_number_++; }

  void set_winner() {
    if (turn_number_ % 2 == 0) {
      winner_ = evaluator_black_.player_name();
    } else {
      winner_ = evaluator_red_.player_name();
    }
  }

  void RunGameInternal() {
    evaluator_red_.Start();
    evaluator_black_.Start();
    while (!IsGameOver()) {
      if (turn_number_ % 2 == 0) {
        evaluator_red_.SelectMove();
      } else {
        evaluator_black_.SelectMove();
      }
      IncrementTurnNumber();
    }
    evaluator_red_.Stop();
    evaluator_black_.Stop();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    set_winner();

    std::cout << winner_ << " won the game." << std::endl << std::flush;
  }
};

int main() {

  GameManager game_manager_{};
  game_manager_.RunGame();

  return 0;
}
