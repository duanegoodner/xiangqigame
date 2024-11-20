#pragma once

#include <iostream>
#include <concepts>


// concept_definitions.hpp
template<typename T, typename Derived>
concept DerivedFromBase = std::is_base_of<Base<Derived>, T>::value;

template<typename T>
concept ImplementsInterface = requires(T a) {
    { a.interface() } -> std::same_as<void>;
};

// base.hpp
template <typename Derived>
class Base {
public:
  void interface() {
    // Calls the implementation from the Derived class
    static_cast<Derived *>(this)->implementation();
  }

  // The Derived class is expected to implement this
  void implementation() { static_cast<Derived *>(this)->implementation(); }
};

// derived.hpp
class Derived : public Base<Derived> {
public:
  void implementation() {
    std::cout << "Custom behavior of Derived class." << std::endl;
  }
};


// consumer.hpp
template<typename T>
requires ImplementsInterface<T>
class Consumer {
private:
    T& base;

public:
    Consumer(T& base) : base(base) {}

    void operate() {
        base.interface();
    }
};
