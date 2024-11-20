#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

struct Move;
struct MoveCollection;

class TranspositionTable {
public:
  void read() const { std::cout << "Reading from TranspositionTable." << std::endl; }

  void write() { std::cout << "Writing to TranspositionTable." << std::endl; }

  void remove_data() {
    std::cout << "Deleting some data in TranspositionTable." << std::endl;
  }
};

class TranspositionTableGuard {
  TranspositionTable &transposition_table_;
  mutable std::shared_mutex mutex_;

public:
  TranspositionTableGuard(TranspositionTable &transposition_table)
      : transposition_table_{transposition_table}
      , mutex_{} {}

  // Template version that accepts any callable with TranspositionTable& as arg
  template <typename Func>
  void OperateDirectlyOnLockedTable(Func &&operation) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    std::cout << "Mutex acquired for direct operation on TranspositionTable"
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    operation(transposition_table_);
    std::cout
        << "Direct operation on TranspositionTableComplete. Mutex will be released."
        << "\n"
        << std::endl;
  }

  template <typename Func>
  void OperateFunctionOfMoveCollection(Func &&operation) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    std::cout << "Mutex acquired to run operation that has MoveCollection as arg."
              << std::endl;
              
  }
};

class TranspositionTablePruner {
  TranspositionTableGuard &tr_table_guard_;
  std::thread pruning_thread_;

public:
  TranspositionTablePruner(TranspositionTableGuard &tr_table_guard)
      : tr_table_guard_{tr_table_guard}
      , pruning_thread_{} {}

  ~TranspositionTablePruner() {
    if (pruning_thread_.joinable()) {
      pruning_thread_.join();
    }
  }

  void Run() {
    std::cout << "Starting the PRUNER" << "\n" << std::endl;
    pruning_thread_ = std::thread(&TranspositionTablePruner::ContinuouslyPrune, this);
  }

private:
  void SinglePrune() {
    std::cout << "PRUNER will attempt to acquire mutex" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    tr_table_guard_.OperateDirectlyOnLockedTable([](TranspositionTable &tr_table) {
      tr_table.read();
      std::this_thread::sleep_for(std::chrono::milliseconds(250));
      tr_table.remove_data();
      std::this_thread::sleep_for(std::chrono::milliseconds(250));
    });
  }

  void ContinuouslyPrune() {
    while (true) {
      SinglePrune();
      std::this_thread::sleep_for(std::chrono::milliseconds(1000)
      ); // Added to reduce CPU usage.
    }
  }
};

class ZobristManager {
  TranspositionTable tr_table_;
  TranspositionTableGuard tr_table_guard_;
  TranspositionTablePruner tr_table_pruner_;

public:
  ZobristManager()
      : tr_table_{}
      , tr_table_guard_{tr_table_}
      , tr_table_pruner_{tr_table_guard_} {}

  void StartPruner() { tr_table_pruner_.Run(); }


};

int main() {

  ZobristManager zobrist_manager{};
  zobrist_manager.StartPruner();

  return 0;
}
