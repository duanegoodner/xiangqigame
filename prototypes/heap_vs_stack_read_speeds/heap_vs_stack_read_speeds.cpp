#include <chrono>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <memory>
#include <cstdlib>
#include <random>

volatile int sink; // Use volatile to avoid compiler optimizations

struct Data {
    int numbers[1024];
};

// Function to consume CPU cache
void consumeCache() {
    static std::vector<int> cachePolluter(30 * 1024 * 1024 / sizeof(int), 0);
    std::generate(cachePolluter.begin(), cachePolluter.end(), std::rand);
    sink = cachePolluter[rand() % cachePolluter.size()];
}

// Function to access Data
void accessData(Data* data) {
    long long sum = 0;
    std::vector<int> indices(1024);
    std::iota(indices.begin(), indices.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g); // Randomize access to simulate real-world usage
    for (int j : indices) {
        sum += data->numbers[j];
    }
    sink = sum;
}

int main() {
    // Stack allocation
    Data stackData;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) {
        accessData(&stackData);
        consumeCache();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> stackDuration = end - start;
    std::cout << "Stack access time: " << stackDuration.count() << " s\n";

    // Heap allocation with regular pointer
    Data* heapData = new Data;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) {
        accessData(heapData);
        consumeCache();
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> heapDuration = end - start;
    std::cout << "Heap access time with regular pointer: " << heapDuration.count() << " s\n";
    delete heapData;

    // Heap allocation with shared_ptr
    auto sharedData = std::make_shared<Data>();
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) {
        accessData(sharedData.get());
        consumeCache();
    }
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sharedDuration = end - start;
    std::cout << "Heap access time with shared_ptr: " << sharedDuration.count() << " s\n";

    return 0;
}
