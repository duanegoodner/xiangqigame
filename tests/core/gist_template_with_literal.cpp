#include <iostream>
#include <random>
#include <array>

template <typename IntType, size_t NumValues>
class MyContainer {
public:
  MyContainer(std::array<IntType, NumValues> data)
      : data_{data} {}
  MyContainer()
      : data_{} {
    for (auto index = 0; index < NumValues; ++index) {
      data_[index] = (std::numeric_limits<IntType>::min() + index);
    }
  }
  size_t size() const { return data_.size(); }

  IntType DataAt(size_t index) const { return data_.at(index); }

private:
  std::array<IntType, NumValues> data_;
};


int main() {
  MyContainer<uint64_t, 3> my_container{};

  for (auto index = 0; index < my_container.size(); ++index) {
    std::cout << my_container.DataAt(index) << std::endl;
  }

  return 0;
}