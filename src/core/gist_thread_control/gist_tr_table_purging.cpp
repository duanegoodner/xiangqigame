#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <thread>
#include <unordered_map>

class Worker {
private:
  std::string name_;
  std::unordered_map<uint32_t, uint32_t> transposition_table_;
  std::unordered_map<uint32_t, std::shared_mutex> mutex_map_;
  std::shared_mutex global_mutex_;
  std::mt19937 rng_;
  std::thread purger_thread_;
  std::atomic<bool> keep_running_;

public:
  Worker(std::string name)
      : name_{name}
      , rng_(std::random_device{}())
      , keep_running_(true) {}

  ~Worker() {
    Stop();
    if (purger_thread_.joinable()) {
      purger_thread_.join();
    }
  }

  void StartPurger() { purger_thread_ = std::thread(&Worker::PurgeEntries, this); }

  void AddEntries() {
    std::lock_guard<std::shared_mutex> lock(global_mutex_);
    
    std::cout << name_ << " beginning to add block of entries -------" << std::endl;
    for (auto i = 0; i < 5; ++i) {
      auto key = (uint32_t)rng_();
      auto value = (uint32_t)rng_() % 10;
      std::cout << name_ << " adding (" << key << ", " << value << std::endl; 
      transposition_table_.insert_or_assign(key, value);
      if (mutex_map_.find(key) == mutex_map_.end()) {
        mutex_map_.try_emplace(key);
      }
    }
    std::cout << name_ << " done adding block of entries -------" << std::endl;
    std::cout << std::endl; 
  }

  void PurgeEntries() {
    while (keep_running_) {
      std::this_thread::sleep_for(std::chrono::microseconds(1));
      std::lock_guard<std::shared_mutex> lock(global_mutex_);
      for (auto it = transposition_table_.begin(); it != transposition_table_.end();) {
        std::unique_lock<std::shared_mutex> lock(mutex_map_[it->first]);
        if (it->second % 7 == 0) {
          std::cout << name_ << " found something to purge!!!!!" << std::endl;
          std::cout << name_ << " purging (" << it->first << ", " << it->second << ")"
                    << std::endl;
          std::cout <<std::endl;
          it = transposition_table_.erase(it);
        } else {
          ++it;
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
  Worker &worker1_;
  Worker &worker2_;
  std::mt19937 rng_; // Use a separate RNG for the manager

public:
  Manager(Worker &w1, Worker &w2)
      : worker1_(w1)
      , worker2_(w2)
      , rng_(std::random_device{}()) {}

  void ManageEntries() {
    uint32_t turn_number = 0;
    while (worker1_.GetTableSize() <= 15 && worker2_.GetTableSize() <= 15) {
      //   int value = rng_();
      if (turn_number % 2 == 0) {
        worker1_.AddEntries();
      } else {
        worker2_.AddEntries();
      }
      turn_number++;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    worker1_.Stop();
    worker2_.Stop();
  }
};

int main() {
  Worker worker1{"Worker-1"}, worker2{"Worker-2"};
  Manager manager(worker1, worker2);

  worker1.StartPurger();
  worker2.StartPurger();

  manager.ManageEntries();

  std::cout << "Final sizes of transposition tables: "
            << "Worker 1 = " << worker1.GetTableSize() << ", "
            << "Worker 2 = " << worker2.GetTableSize() << std::endl;

  return 0;
}
