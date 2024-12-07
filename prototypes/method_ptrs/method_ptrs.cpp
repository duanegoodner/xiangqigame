#include <functional>
#include <iostream>
#include <vector>

// Define classes with member functions
class A {
public:
    void method(int x) {
        std::cout << "A::method: " << x << std::endl;
    }
};

class B {
public:
    void method(int x) {
        std::cout << "B::method: " << x << std::endl;
    }
};

class C {
public:
    void method(int x) {
        std::cout << "C::method: " << x << std::endl;
    }
};

// Define a templated structure for the member function pointer and the object pointer
template <typename T>
struct Method {
    T* object;
    void (T::*method)(int);

    Method(T* obj, void (T::*meth)(int)) : object(obj), method(meth) {}

    void call(int x) {
        (object->*method)(x); // Invoke the method on the object
    }
};

int main() {
    A a;
    B b;
    C c;

    // Create a vector of Method objects for different types
    std::vector<std::function<void(int)>> methods = {
        [&a](int x) { a.method(x); },
        [&b](int x) { b.method(x); },
        [&c](int x) { c.method(x); }
    };

    // Call each method
    for (auto& method : methods) {
        method(42);
    }

    return 0;
}
