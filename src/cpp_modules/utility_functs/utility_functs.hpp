#ifndef _UTILITY_FUNCTS_
#define _UTILITY_FUNCTS_

#include <random>

namespace utility_functs {

template<typename T>
T random(T range_from, T range_to) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<T>    distr(range_from, range_to);
    return distr(generator);
}


}  // namespace utility_functs

#endif  // _UTILITY_FUNCTS_
