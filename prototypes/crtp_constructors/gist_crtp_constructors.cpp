#include <iostream>

template<typename Derived>
class Base {
public:
    Base() {
        // Constructor logic here
        // Can use static_cast to call methods that are expected to be implemented by the Derived class
        static_cast<Derived*>(this)->doInitialize();
    }

    void interfaceMethod() {
        static_cast<Derived*>(this)->implementationMethod();
    }
};

class Derived : public Base<Derived> {
public:
    // Derived() {
    //     // Derived-specific construction logic can also be placed here.
    // }

    void doInitialize() {
        std::cout << "Derived initialization logic here." << std::endl;
        // Initialization specific to Derived
    }

    void implementationMethod() {
        std::cout << "Implementation of a method required by the Base." << std::endl;
        // Actual implementation of a method required by the Base
    }
};

int main() {
    // Create an instance of Derived
    Derived derived;

    // Call a method defined in the Base that will invoke a method in Derived
    derived.interfaceMethod();

    return 0;
}
