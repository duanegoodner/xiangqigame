#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <thread>
#include <unordered_map>

class Player {
private:
  std::string name_;
  std::unordered_map<uint32_t, uint32_t> transposition_table_;
  std::shared_mutex global_mutex_;
  std::mt19937 rng_;
  std::thread purger_thread_;
  std::atomic<bool> keep_running_;

public:
  Player(std::string name)
      : name_{name}
      , rng_(std::random_device{}())
      , keep_running_(true) {}

  ~Player() {
    Stop();
    if (purger_thread_.joinable()) {
      purger_thread_.join();
    }
  }

  void StartPruner() { purger_thread_ = std::thread(&Player::PruneEntries, this); }

  void EvaluateNode() {

    std::this_thread::sleep_for(std::chrono::microseconds(25));
    auto board_state = (uint32_t)rng_() % 200;
    auto current_result = (uint32_t)rng_() % 42;
    auto transposition_table_it = transposition_table_.find(board_state);
    if (transposition_table_it != transposition_table_.end()) {
      auto previous_result = transposition_table_it->second;
      if (current_result <= previous_result) {
        std::cout << name_ << " kept (" << board_state << ", " << previous_result
                  << " instead of writing (" << board_state << ", " << current_result
                  << ")" << std::endl;
        return;
      }
    }
    transposition_table_.insert_or_assign(board_state, current_result);
    std::cout << name_ << " added (" << board_state << ", " << current_result << ")"
              << std::endl;
  }

  void EvaluateNodes() {
    std::lock_guard<std::shared_mutex> lock(global_mutex_);

    std::cout << name_ << " beginning to add block of entries -------" << std::endl;
    for (auto i = 0; i < 5; ++i) {
      EvaluateNode();
    }
    std::cout << name_ << " done adding block of entries -------" << std::endl;
    std::cout << std::endl;
  }

  void PruneEntries() {
    while (keep_running_) {
      std::this_thread::sleep_for(std::chrono::microseconds(5));
      std::lock_guard<std::shared_mutex> lock(global_mutex_);
      if (!transposition_table_.empty()) {
        std::uniform_int_distribution<size_t> distribution(
            0,
            transposition_table_.size() - 1
        );
        auto it = transposition_table_.begin();
        std::advance(it, distribution(rng_));

        if (it->second % 7 == 0) {
          std::cout << name_ << " found something to prune!!!!!" << std::endl;
          std::cout << name_ << " purging (" << it->first << ", " << it->second << ")"
                    << std::endl;
          std::cout << std::endl;
          it = transposition_table_.erase(it);
        }
      }
    }
  }

  void Stop() { keep_running_ = false; }

  size_t GetTableSize() {
    std::shared_lock<std::shared_mutex> lock(global_mutex_);
    return transposition_table_.size();
  }
};

class Manager {
private:
  Player &player1_;
  Player &player2_;

public:
  Manager(Player &e1, Player &e2)
      : player1_(e1)
      , player2_(e2) {}

  void ManageEvaluators() {
    uint32_t turn_number = 0;
    while (player1_.GetTableSize() <= 15 && player2_.GetTableSize() <= 15) {
      if (turn_number % 2 == 0) {
        player1_.EvaluateNodes();
      } else {
        player2_.EvaluateNodes();
      }
      turn_number++;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    player1_.Stop();
    player2_.Stop();
  }
};

int main() {
  Player player1{"Player-1"}, player2{"Player-2"};
  Manager manager(player1, player2);

  player1.StartPruner();
  player2.StartPruner();

  manager.ManageEvaluators();

  std::cout << "Final sizes of transposition tables: "
            << "Player 1 = " << player1.GetTableSize() << ", "
            << "Player 2 = " << player2.GetTableSize() << std::endl;

  return 0;
}
