#include <concepts>
#include <functional>
#include <iostream>

struct SomeAction {
  int start;
  int end;
};

template <typename KeyType>
class ZobristItem {
  KeyType key_;

public:
  void DoSomethingBasedOnAction(const SomeAction &action) {
    std::cout << "Updating some private data structs." << std::endl;
  }
};

class ActionCenter {
  std::function<void(const SomeAction &)> func_a_;
  std::function<void(const SomeAction &)> func_b_;

public:
  ActionCenter(
      std::function<void(const SomeAction &)> func_a,
      std::function<void(const SomeAction &)> func_b
  )
      : func_a_(func_a)
      , func_b_(func_b) {}

  void MakeThingsHappen(const SomeAction &action) {
    func_a_(action);
    func_b_(action);
    InternalThingsAtTheActionCenter(action);
  }

private:
  void InternalThingsAtTheActionCenter(const SomeAction &action) {
    std::cout << "Doing internal work at the action center." << std::endl;
  }
};

int main() {
  ZobristItem<uint32_t> zobrist_item_a;
  ZobristItem<uint64_t> zobrist_item_b;

  // Create std::function objects pointing to the DoSomethingBasedOnAction method of each
  // ZobristItem instance
  ActionCenter action_center{
      std::bind(
          &ZobristItem<uint32_t>::DoSomethingBasedOnAction,
          &zobrist_item_a,
          std::placeholders::_1
      ),
      std::bind(
          &ZobristItem<uint64_t>::DoSomethingBasedOnAction,
          &zobrist_item_b,
          std::placeholders::_1
      )
  };

  SomeAction some_action{1, 2};
  action_center.MakeThingsHappen(some_action);

  return 0;
}
