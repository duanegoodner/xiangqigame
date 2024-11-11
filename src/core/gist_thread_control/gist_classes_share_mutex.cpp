#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

class Resource {
public:
    void read() const {
        std::cout << "Reading from resource." << std::endl;
    }

    void write() {
        std::cout << "Writing to resource." << std::endl;
    }
};

class Child {
    mutable std::shared_mutex mutex_;
    Resource resource_;

public:
    Child() {}

    // Template version that accepts any callable
    template<typename Func>
    void withLockedResource(Func&& operation) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        operation(resource_);
    }
};

class Parent {
    Child& child;

public:
    explicit Parent(Child& c) : child(c) {}

    void performComplexOperation() {
        child.withLockedResource([](Resource &res) {
            res.read();
            res.write();
        });
    }
};

int main() {
    Child child;
    Parent parent(child);

    std::thread parentThread(&Parent::performComplexOperation, &parent);
    parentThread.join();

    return 0;
}
