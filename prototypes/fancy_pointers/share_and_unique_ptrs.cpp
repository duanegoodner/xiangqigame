#include <iostream>
#include <memory>

class Dependency {
public:
  Dependency() { std::cout << "Dependency constructor\n"; }
  ~Dependency() { std::cout << "Dependency destructor\n"; }
  void operation(std::string message) { std::cout << message << std::endl; }
};

int main() {

  std::shared_ptr<Dependency> outerScopePtr;

  {
    std::unique_ptr<Dependency> uniqueDep = std::make_unique<Dependency>();
    uniqueDep->operation(
        "This message is called from a Dependency instance owned by a unique_ptr."
    ); // Performs an operation
       // uniqueDep automatically deleted here
  } // uniqueDep goes out of scope and its destructor is automatically called

  {
    std::shared_ptr<Dependency> sharedDep = std::make_shared<Dependency>();
    outerScopePtr = sharedDep; // shared ownership
    sharedDep->operation(
        "This message is called from a Depenency instance owned by a shared_ptr."
    ); // Performs the same operation
    // sharedDep is not deleted here because anotherSharedDep still exists
  } // Both sharedDep and anotherSharedDep go out of scope, now the object is deleted

  outerScopePtr->operation("Hooray, one of the pointers who owns me is still in scope, "
                           "so I can send this message too!");

  return 0;
}
