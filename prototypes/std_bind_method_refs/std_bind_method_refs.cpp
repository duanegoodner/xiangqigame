#include <functional>
#include <iostream>
#include <memory>
#include <vector>

struct Action {};

template <typename KeyType>
class ZCalculator {
  KeyType key_val_;

public:
  KeyType key_val() { return key_val_; }
  void UpdateKeyVal(const Action &action) { key_val_++; }
};

class ActionTracker {
  std::vector<std::function<void(const Action &)>> action_callbacks_;

public:
  void AttachActionCallback(const std::function<void(const Action &)> &callback) {
    action_callbacks_.emplace_back(callback);
  }
  void ProcessAction(const Action &action) {
    for (const auto &callback : action_callbacks_) {
      callback(action);
    }
  }
};

int main() {

  ZCalculator<u_int64_t> calculator_a{};
  ZCalculator<u_int32_t> calculator_b{};

  auto calculator_c_ptr = std::make_shared<ZCalculator<uint16_t>>();

  ActionTracker action_tracker;

  action_tracker.AttachActionCallback(std::bind(
      &ZCalculator<u_int64_t>::UpdateKeyVal,
      &calculator_a,
      std::placeholders::_1
  ));

  action_tracker.AttachActionCallback(std::bind(
      &ZCalculator<u_int32_t>::UpdateKeyVal,
      &calculator_b,
      std::placeholders::_1
  ));

  action_tracker.AttachActionCallback(std::bind(
    &ZCalculator<u_int16_t>::UpdateKeyVal,
    calculator_c_ptr,
    std::placeholders::_1
  ));

  Action action_1{};
  Action action_2{};

  std::cout << calculator_a.key_val() << std::endl;
  std::cout << calculator_b.key_val() << std::endl;
  std::cout << calculator_c_ptr->key_val() << std::endl;

  action_tracker.ProcessAction(action_1);

  std::cout << calculator_a.key_val() << std::endl;
  std::cout << calculator_b.key_val() << std::endl;
  std::cout << calculator_c_ptr->key_val() << std::endl;

  action_tracker.ProcessAction(action_2);

  std::cout << calculator_a.key_val() << std::endl;
  std::cout << calculator_b.key_val() << std::endl;
  std::cout << calculator_c_ptr->key_val() << std::endl;

  return 0;
}