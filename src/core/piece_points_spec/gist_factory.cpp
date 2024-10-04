#include <iostream>
#include <functional>
#include <memory>

// Base class
class Item {
public:
    virtual void use() = 0;  // Pure virtual function
    virtual ~Item() {}
};

// Concrete implementation A
class ItemA : public Item {
public:
    void use() override {
        std::cout << "Using ItemA" << std::endl;
    }
};

// Concrete implementation B
class ItemB : public Item {
public:
    void use() override {
        std::cout << "Using ItemB" << std::endl;
    }
};


class User {
private:
    std::unique_ptr<Item> item;

public:
    // Constructor accepts a factory function that returns a unique_ptr to an Item
    User(std::function<std::unique_ptr<Item>()> itemFactory) : item(itemFactory()) {}

    void useItem() {
        item->use();
    }
};


int main() {
    // Create a User with ItemA
    User userA([]() { return std::make_unique<ItemA>(); });
    userA.useItem();

    // Create a User with ItemB
    User userB([]() { return std::make_unique<ItemB>(); });
    userB.useItem();

    return 0;
}
