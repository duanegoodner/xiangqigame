#include <iostream>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <thread>
#include <unordered_map>

class TranspositionTableSearchResult {
public:
  bool found() { return found_; }
  void set_found(bool status) { found_ = found; }

  uint32_t value() { return value_; }
  void set_value(uint32_t value) { value_ = value; }

private:
  bool found_;
  uint32_t value_;
};

// Resource
class TranspositionTable {
public:
  TranspositionTable() {}

  uint32_t GetDataAt(uint32_t key, TranspositionTableSearchResult &result) {
    auto entry_it = data_.find(key);
    if (entry_it != data_.end()) {
      result.set_found(true);
      result.set_value(entry_it->second);
    }
  }

  void WriteData(uint32_t key, uint32_t value) {
    data_.insert_or_assign(key, value);
  }

private:
  std::unordered_map<uint32_t, uint32_t> data_;
};

// Child
class ZobristTracker {
public:
  ZobristTracker() {}

  template <typename TrTableOp>
  void WithLockedTrTable(TrTableOp &&operation) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    operation(transposition_table_);
  }

private:
  TranspositionTable transposition_table_;
  mutable std::shared_mutex mutex_;
};

// Parent
class MinimaxEvaluator {};
