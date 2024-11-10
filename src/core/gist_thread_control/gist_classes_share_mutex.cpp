#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <functional>

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

    // Provide a method that locks and allows operations to be performed on the resource
    void withLockedResource(const std::function<void(Resource&)> &operation) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        operation(resource_);
    }

    // These methods assume locking is handled elsewhere or are not exposed publicly if not needed
    void accessResource() const {
        resource_.read();
    }

    void modifyResource() {
        resource_.write();
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
