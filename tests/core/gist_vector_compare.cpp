#include <algorithm> // for std::find
#include <iostream>
#include <vector>

struct MyStruct {
  int val;

  bool operator==(const MyStruct &other) const { return val == other.val; }
};

bool is_acceptable_tr_table_result(
    const std::vector<MyStruct> &allowed_items,
    const std::vector<MyStruct> &tr_table_items
) {
  if (tr_table_items.empty() != allowed_items.empty()) {
    return false;
  }
  if (tr_table_items.empty() and allowed_items.empty()) {
    return true;
  }

  for (const auto &tr_table_item : tr_table_items) {
    if (std::find(allowed_items.begin(), allowed_items.end(), tr_table_item) == allowed_items.end()) {
      return false;
    }
  }
  return true;
}

int main() {
  // Test case 1: Allowed and TrTableResult are both empty
  std::vector<MyStruct> allowed_1;
  std::vector<MyStruct> tr_table_1;
  std::cout << "Test case 1 (both Allowed and TrTableResult empty): "
            << (is_acceptable_tr_table_result(allowed_1, tr_table_1) ? "True" : "False") << std::endl;

  // Test case 2: A is empty, TrTableResult has elements
  std::vector<MyStruct> allowed_2;
  std::vector<MyStruct> tr_table_2 = {{1}, {2}};
  std::cout << "Test case 2 (Allowed empty, TrTableResult has elements): "
            << (is_acceptable_tr_table_result(allowed_2, tr_table_2) ? "True" : "False") << std::endl;

  // Test case 3: TrTableResult is empty, Allowed has elements
  std::vector<MyStruct> allowed_3 = {{1}, {2}, {3}};
  std::vector<MyStruct> tr_table_3;
  std::cout << "Test case 3 (TrTableResult empty, Allowed has elements): "
            << (is_acceptable_tr_table_result(allowed_3, tr_table_3) ? "True" : "False") << std::endl;

  // Test case 4: All elements in TrTableResult are in Allowed
  std::vector<MyStruct> allowed_4 = {{1}, {2}, {3}, {4}};
  std::vector<MyStruct> tr_table_4 = {{2}, {3}};
  std::cout << "Test case 4 (all elements in TrTableResult are in Allowed): "
            << (is_acceptable_tr_table_result(allowed_4, tr_table_4) ? "True" : "False") << std::endl;

  // Test case 5: TrTableResult has some elements not in Allowed
  std::vector<MyStruct> allowed_5 = {{1}, {2}, {3}};
  std::vector<MyStruct> tr_table_5 = {{2}, {4}};
  std::cout << "Test case 5 (TrTableResult has elements not in Allowed): "
            << (is_acceptable_tr_table_result(allowed_5, tr_table_5) ? "True" : "False") << std::endl;

  // Test case 6: All elements in TrTableResult are not in Allowed
  std::vector<MyStruct> allowed_6 = {{1}, {2}};
  std::vector<MyStruct> tr_table_6 = {{3}, {4}};
  std::cout << "Test case 6 (all elements in TrTableResult are not in Allowed): "
            << (is_acceptable_tr_table_result(allowed_6, tr_table_6) ? "True" : "False") << std::endl;

  return 0;
}
