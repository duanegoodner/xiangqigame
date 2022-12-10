#include <iostream>
#include <utility>

template <typename D>
struct Base
{

    void common()
    {
        static_cast<D *>(this)->impl();
    }

    template <typename T>
    int common_with_arg(T &&arg)
    {
        return static_cast<D *>(this)->impl_with_arg(std::forward<T>(arg));
    }
};


struct Derived : Base<Derived>
{

    int fixed_int_;

    Derived(int fixed_int);
    void impl();
    

    // using Arg = int;
    int impl_with_arg(int arg);
};