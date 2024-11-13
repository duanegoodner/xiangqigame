#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

class TranspositionTable {
public:
  void read() const { std::cout << "Reading from TranspositionTable." << std::endl; }

  void write() { std::cout << "Writing to TranspositionTable." << std::endl; }

  void remove_data() {
    std::cout << "Deleting some data in TranspositionTable." << std::endl;
  }
};



class TranspositionTablePruner {
  TranspositionTable &tr_table_;
  std::shared_mutex &tr_table_mutex_;
  std::thread pruning_thread_;

public:
  TranspositionTablePruner(
      TranspositionTable &tr_table,
      std::shared_mutex &tr_table_mutex
  )
      : tr_table_{tr_table}
      , tr_table_mutex_{tr_table_mutex}
      , pruning_thread_{} {
    Run();
  }

  void Prune() {
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    std::unique_lock<std::shared_mutex> lock(tr_table_mutex_);
    std::cout << "PRUNER has the mutex." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    tr_table_.read();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    tr_table_.remove_data();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::cout << "PRUNER releasing the mutex." << "\n" << std::endl;
  }

  void ContinuouslyPrune() {
    while (true) {
      Prune();
    }
  }

  void Run() {
    std::cout << "Starting the PRUNER" << "\n" << std::endl;
    pruning_thread_ = std::thread(&TranspositionTablePruner::ContinuouslyPrune, this);
  }
};

class ZobristManager {
  mutable std::shared_mutex tr_table_mutex_;
  TranspositionTable tr_table_;
  TranspositionTablePruner tr_table_pruner_;

public:
  ZobristManager()
      : tr_table_mutex_{}
      , tr_table_{}
      , tr_table_pruner_{TranspositionTablePruner{tr_table_, tr_table_mutex_}} {}

  std::shared_mutex &tr_table_mutex() { return tr_table_mutex_; }

  void ReadResource() { tr_table_.read(); }
  void WriteResource() { tr_table_.write(); }
};

class GameBoard {
public:
  void PerformAction() { std::cout << "Performing action in GameBoard." << std::endl; }
};

class GamePositionPoints {
public:
  void ReadPointsData() const {
    std::cout << "Reading points data from GamePositionPoints." << std::endl;
  }
};

class MinimaxMoveEvaluator {
  GameBoard &game_board_;
  GamePositionPoints game_position_points_;
  ZobristManager zobrist_manager_;

public:
  MinimaxMoveEvaluator(GameBoard &game_board)
      : game_board_{game_board}
      , game_position_points_{GamePositionPoints{}}
      , zobrist_manager_{ZobristManager{}} {}

  void ComplexOperation() {
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    std::unique_lock<std::shared_mutex> lock(zobrist_manager_.tr_table_mutex());
    std::cout << "EVALUATOR has the mutex." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    game_board_.PerformAction();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    zobrist_manager_.ReadResource();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    game_position_points_.ReadPointsData();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    zobrist_manager_.WriteResource();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::cout << "EVALUATOR releasing the mutex." << "\n" << std::endl;
  }

  void ContinuouslyRunComplexOperation() {
    while (true) {
      ComplexOperation();
    }
  }
};

int main() {
  GameBoard game_board;
  MinimaxMoveEvaluator move_evaluator{game_board};

  move_evaluator.ContinuouslyRunComplexOperation();
}
