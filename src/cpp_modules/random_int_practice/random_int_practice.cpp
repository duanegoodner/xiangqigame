#include <iostream>
#include <random>
#include <string>
#include <vector>

template<typename T>
T random(T range_from, T range_to) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<T>    distr(range_from, range_to);
    return distr(generator);
}

struct Thing {
    std::string name;
    int val;
};

int main()
{    
    
    auto thing_a = Thing{"one", 1};
    auto thing_b = Thing{"two", 2};
    auto thing_c = Thing{"three", 3};

    std::vector<Thing> my_things = {thing_a, thing_b, thing_c};

    auto index = random<size_t>(0, my_things.size() - 1);

    auto result = my_things[index];

    std::cout << index << std::endl;
    std::cout << result.name << std::endl;

    return 0;
    
    // const int range_from  = 0;
    // const int range_to    = 1000;
    // std::random_device                  rand_dev;
    // std::mt19937                        generator(rand_dev());
    // std::uniform_int_distribution<int>  distr(range_from, range_to);

    // std::mt19937                        generator_2(rand_dev());


    // std::cout << distr(generator) << '\n';
    // std::cout << distr(generator_2) << '\n';
}