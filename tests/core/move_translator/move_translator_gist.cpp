#include <vector>
#include <iostream>
#include <ranges>

int main() {
    std::vector<int> vec = {10, 20, 30, 40, 50};

    // // Reference to avoid unnecessary copying
    // for (auto& [index, value] : std::views::enumerate(vec)) {
    //     std::cout << "Index: " << index << ", Value: " << value << '\n';
    // }

    return 0;
}
