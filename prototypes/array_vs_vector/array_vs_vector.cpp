#include <iostream>
#include <array>
#include <vector>
#include <chrono>

struct Data {
    int num;
    double val;
    char arr[256];
};

int main() {
    constexpr int iterations = 1000000; // Number of read iterations to perform

    std::array<Data, 2> dataArray = {{{1, 2.3, "First"}, {4, 5.6, "Second"}}};
    std::vector<Data> dataVector = {{1, 2.3, "First"}, {4, 5.6, "Second"}};

    auto startArray = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        volatile auto a = dataArray[0];
        volatile auto b = dataArray[1];
    }
    auto endArray = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> arrayDuration = endArray - startArray;
    
    auto startVector = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        volatile auto a = dataVector[0];
        volatile auto b = dataVector[1];
    }
    auto endVector = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> vectorDuration = endVector - startVector;

    std::cout << "Time taken by std::array: " << arrayDuration.count() << " milliseconds\n";
    std::cout << "Time taken by std::vector: " << vectorDuration.count() << " milliseconds\n";

    return 0;
}
