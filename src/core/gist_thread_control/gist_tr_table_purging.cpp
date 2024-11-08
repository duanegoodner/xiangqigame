#include <iostream>
#include <unordered_map>
#include <shared_mutex>
#include <chrono>
#include <thread>
#include <mutex>
#include <random>
#include <atomic>

using big_unsigned_integer = unsigned long long;

class Worker {
 private:
  std::unordered_map<big_unsigned_integer, int> transposition_table_;
  std::unordered_map<big_unsigned_integer, std::shared_mutex> mutex_map_;
  std::shared_mutex global_mutex_;
  std::mt19937 rng_;
  std::thread purger_thread_;
  std::atomic<bool> keep_running_;

 public:
  Worker() : rng_(std::random_device{}()), keep_running_(true) {}

  ~Worker() {
    Stop();
    if (purger_thread_.joinable()) {
      purger_thread_.join();
    }
  }

  void StartPurger() {
    purger_thread_ = std::thread(&Worker::PurgeEntries, this);
  }

  void AddEntry(big_unsigned_integer key, int value) {
    std::lock_guard<std::shared_mutex> lock(global_mutex_);
    transposition_table_.insert_or_assign(key, value);
    if (mutex_map_.find(key) == mutex_map_.end()) {
      mutex_map_.try_emplace(key);
    }
  }

  void PurgeEntries() {
    while (keep_running_) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::lock_guard<std::shared_mutex> lock(global_mutex_);
      for (auto it = transposition_table_.begin(); it != transposition_table_.end();) {
        std::unique_lock<std::shared_mutex> lock(mutex_map_[it->first]);
        if (it->second % 7 == 0) {
          it = transposition_table_.erase(it);
        } else {
          ++it;
        }
      }
    }
  }

  void Stop() {
    keep_running_ = false;
  }

  size_t GetTableSize() {
    std::shared_lock<std::shared_mutex> lock(global_mutex_);
    return transposition_table_.size();
  }
};


class Manager {
 private:
  Worker& worker1_;
  Worker& worker2_;
  std::mt19937 rng_;  // Use a separate RNG for the manager

 public:
  Manager(Worker& w1, Worker& w2) : worker1_(w1), worker2_(w2), rng_(std::random_device{}()) {}

  void ManageEntries() {
    big_unsigned_integer key = 0;
    while (worker1_.GetTableSize() <= 100 && worker2_.GetTableSize() <= 100) {
      int value = rng_();
      if (key % 2 == 0) {
        worker1_.AddEntry(key, value);
      } else {
        worker2_.AddEntry(key, value);
      }
      key++;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    worker1_.Stop();
    worker2_.Stop();
  }
};


int main() {
  Worker worker1, worker2;
  Manager manager(worker1, worker2);

  worker1.StartPurger();
  worker2.StartPurger();

  manager.ManageEntries();

  std::cout << "Final sizes of transposition tables: "
            << "Worker 1 = " << worker1.GetTableSize() << ", "
            << "Worker 2 = " << worker2.GetTableSize() << std::endl;

  return 0;
}



