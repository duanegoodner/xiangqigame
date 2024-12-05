#include <iostream>
#include <memory>

// Define ConceptA
template<typename T>
concept ConceptA = requires(T t) {
    { t.operationA() } -> std::same_as<void>;
};

// Define ConceptB
template<typename T>
concept ConceptB = requires(T t) {
    { t.operationB() } -> std::same_as<void>;
};

// Define ConceptAB as requiring both ConceptA and ConceptB
template<typename T>
concept ConceptAB = ConceptA<T> && ConceptB<T>;

// Class that implements both ConceptA and ConceptB
class MyClass {
public:
    void operationA() {
        std::cout << "Performing operation A" << std::endl;
    }
    void operationB() {
        std::cout << "Performing operation B" << std::endl;
    }
};

// ConsumerA only needs ConceptA
template<ConceptA T>
class ConsumerA {
public:
    explicit ConsumerA(std::shared_ptr<T> ptr) {
        ptr->operationA();
    }
};

// ConsumerB only needs ConceptB
template<ConceptB T>
class ConsumerB {
public:
    explicit ConsumerB(std::shared_ptr<T> ptr) {
        ptr->operationB();
    }
};

// Builder class
template<ConceptAB T>
class MyBuilder {
private:
    std::shared_ptr<T> myClassInstance;

public:
    explicit MyBuilder(std::shared_ptr<T> myClass) : myClassInstance(myClass) {}

    std::shared_ptr<ConsumerA<T>> buildConsumerA() {
        return std::make_shared<ConsumerA<T>>(myClassInstance);
    }

    std::shared_ptr<ConsumerB<T>> buildConsumerB() {
        return std::make_shared<ConsumerB<T>>(myClassInstance);
    }
};

// Main function to demonstrate usage
int main() {
    auto myClassInstance = std::make_shared<MyClass>();
    MyBuilder<MyClass> builder(myClassInstance);

    auto consumerA = builder.buildConsumerA();
    auto consumerB = builder.buildConsumerB();

    return 0;
}
