// Declare a base_traits traits class template:
#include <iostream>

template <typename derived_t> 
struct base_traits;

// Define the base class that uses the traits:
template <typename derived_t> 
struct base { 
    typedef typename base_traits<derived_t>::value_type value_type;
    value_type base_foo() {
        return base_traits<derived_t>::call_foo(static_cast<derived_t*>(this));
    }
};

// Define the derived class; it can use the traits too:
template <typename T>
struct derived : base<derived<T> > { 
    typedef typename base_traits<derived>::value_type value_type;

    value_type derived_foo() { 
        return value_type(); 
    }
};

// Declare and define a base_traits specialization for derived:
template <typename T> 
struct base_traits<derived<T> > {
    typedef T value_type;

    static value_type call_foo(derived<T>* x) { 
        return x->derived_foo(); 
    }
};

int main() {
    derived<int> a;
    a.derived_foo();

    return 0;
}