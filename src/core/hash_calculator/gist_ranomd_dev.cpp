#include <random>
#include <iostream>


int main() {

    auto result_a = std::random_device{}();
    auto result_b = std::random_device{}();
    auto result_c = std::random_device{}();
    auto result_d = std::random_device{}();
    auto result_e = std::random_device{}();
    auto result_f = std::random_device{}();

    std::cout << result_a << std::endl;
    std::cout << result_b << std::endl;
    std::cout << result_c << std::endl;
    std::cout << result_d << std::endl;
    std::cout << result_e << std::endl;
    std::cout << result_f << std::endl;


    return 0;
}