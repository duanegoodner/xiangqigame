#pragma once

#include <iostream>
#include <string>

// MyTemplateClass definition
template <typename T>
class MyTemplateClass {
public:
    // Constructor
    MyTemplateClass() {
        std::cout << "MyTemplateClass<" << typeid(T).name() << "> instance created." << std::endl;
    }

    // Member function to demonstrate functionality
    void doSomething() const {
        std::cout << "Doing something with MyTemplateClass<" << typeid(T).name() << ">." << std::endl;
    }

    // Member function to set a value
    void setValue(const T& value) {
        this->value = value;
    }

    // Member function to get the stored value
    T getValue() const {
        return value;
    }

private:
    T value; // A stored value of the template type
};
