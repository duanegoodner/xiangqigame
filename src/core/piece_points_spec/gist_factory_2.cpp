#include <iostream>
#include <memory>

// Abstract base class
class Item {
public:
    virtual ~Item() = default;
    virtual void performAction() const = 0;
};

// Concrete implementation of Item
class ItemA : public Item {
public:
    void performAction() const override {
        std::cout << "Action performed by ItemA" << std::endl;
    }
};

// Factory interface for creating items
class ItemFactory {
public:
    virtual ~ItemFactory() = default;
    virtual std::unique_ptr<Item> createItem() const = 0;
};

// Concrete factory for creating instances of ItemA
class ItemAFactory : public ItemFactory {
public:
    std::unique_ptr<Item> createItem() const override {
        return std::make_unique<ItemA>();
    }
};

// Class that uses Item objects
class User {
private:
    std::unique_ptr<Item> item;

public:
    // Constructor deletion to prevent misuse
    User(const User&) = delete;
    User& operator=(const User&) = delete;

    // User constructor that accepts an ItemFactory
    explicit User(const ItemFactory& factory = ItemAFactory())
        : item(factory.createItem()) {}

    // Function to use the item
    void useItem() const {
        item->performAction();
    }
};

int main() {
    // Default construction leads to usage of ItemA
    User defaultUser;
    defaultUser.useItem(); // Outputs "Action performed by ItemA"

    // Explicitly using factory to create user
    ItemAFactory factory;
    User user(factory);
    user.useItem(); // Outputs "Action performed by ItemA"
    
    return 0;
}
