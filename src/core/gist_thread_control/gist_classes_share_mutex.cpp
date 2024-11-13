#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

// Example Resource and OtherObject classes
class Resource {
public:
    void read() const {
        std::cout << "Reading from resource." << std::endl;
    }

    void write() {
        std::cout << "Writing to resource." << std::endl;
    }
};

class OtherObject {
public:
    void performAction() {
        std::cout << "Performing action on other object." << std::endl;
    }
};

// Child class providing locking mechanism
class Child {
    mutable std::shared_mutex mutex_;

public:
    Child() {}

    // Generalized template method that works with any object passed externally
    template<typename Object, typename Func>
    void withLockedResource(Object& object, Func&& operation) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        operation(object); // Executes the passed function under lock
    }
};

// Parent class orchestrating operations
class Parent {
    Child& child;

public:
    explicit Parent(Child& c) : child(c) {}

    template<typename Object, typename Func>
    void performOperation(Object& object, Func&& operation) {
        child.withLockedResource(object, std::forward<Func>(operation));
    }
};

int main() {
    Child child;
    Parent parent(child);
    Resource resource;
    OtherObject otherObject;

    // Operation on Resource
    std::thread resourceThread([&]() {
        parent.performOperation(resource, [](Resource &res) {
            res.write();
            res.read();
        });
    });

    // Operation on OtherObject
    std::thread otherObjectThread([&]() {
        parent.performOperation(otherObject, [](OtherObject &obj) {
            obj.performAction();
        });
    });

    resourceThread.join();
    otherObjectThread.join();

    return 0;
}
