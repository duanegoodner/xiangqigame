#include <iostream>

template <typename ConcreteWorker>
class Worker {
public:
  // General template for ProvideValue
  template <typename T>
  void ProvideValue(T& result) {
    return static_cast<ConcreteWorker*>(this)->ImplementProvideValue(result);
  }

  // Overloaded function for `int`
  void ProvideValue(int& result) {
    return static_cast<ConcreteWorker*>(this)->ImplementProvideValue(result);
  }
};

class WorkerA : public Worker<WorkerA> {
public:
  // General implementation of ImplementProvideValue for any type T
  template <typename T>
  void ImplementProvideValue(T& result) {
    result = 1;
  }

  // Overloaded implementation for `int`
  void ImplementProvideValue(int& result) {
    result = 1000;
  }
};

int main() {
    WorkerA worker;
    int a;
    worker.ProvideValue(a);
    std::cout << "Provided value for int: " << a << std::endl;

    double b;
    worker.ProvideValue(b);
    std::cout << "Provided value for double: " << b << std::endl;

    return 0;
}
