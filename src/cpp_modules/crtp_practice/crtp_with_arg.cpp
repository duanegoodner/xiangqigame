#include <iostream>
#include <utility>
#include "crtp_with_arg.hpp"


Derived::Derived(int fixed_int) : fixed_int_{fixed_int} {};

void Derived::impl()
    {
        std::cout << "side effect of void function without artgs" << std::endl;
    }

int Derived::impl_with_arg(int arg) {
    return arg + fixed_int_;
}

int main()
{

    auto a = Derived(22);

    a.impl();

    auto result = a.impl_with_arg(1);
    std::cout << "Return val of function with arg: " << result << std::endl;

    return 0;
}