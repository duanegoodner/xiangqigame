#include <iostream>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

class Worker {

public:
  Worker(std::string name)
      : name_{name}
      , prng_{std::random_device{}()}
      , data_{}
      , run_pruner_{true} {}

  size_t data_size() { return data_.size(); }

  void update_data() {
    auto key = (uint32_t)prng_();
    std::lock_guard<std::shared_mutex> lock(full_table_mutex_);
    data_.insert_or_assign(key, (uint32_t)prng_());
    if (mutex_map_.find(key) == mutex_map_.end()) {
      mutex_map_.try_emplace(key, std::shared_mutex{});
    }
  }

private:
  std::string name_;
  std::mt19937 prng_;
  std::unordered_map<uint32_t, uint32_t> data_;
  std::unordered_map<uint32_t, std::shared_mutex> mutex_map_;
  std::shared_mutex full_table_mutex_;
  bool run_pruner_;

  void Prune() {

    while (run_pruner_) {
      std::this_thread::sleep_for(std::chrono::seconds(10));
      std::lock_guard<std::shared_mutex> full_table_lock(full_table_mutex_
      ); // Synchronize access globally

      for (auto it = data_.begin(); it != data_.end();) {
        std::unique_lock<std::shared_mutex> entry_lock(mutex_map_[it->first]);
        auto is_divisible_by_seven = (it->second % 7) == 0;
        if (is_divisible_by_seven) {
          it = data_.erase(it);
        } else {
          ++it;
        }
      }
    }
  }
};

class Manager {
  
};

int main() {
  Worker joe{"Joe"};

  return 0;
}