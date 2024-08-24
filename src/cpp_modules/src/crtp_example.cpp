#include <iostream>

struct SomeType {
  int value;
};

template <typename GenericConcreteClassName>
class InterfaceName {
public:
  SomeType InterfaceMethod(SomeType &some_type) {
    static_cast<GenericConcreteClassName *>(this)->ImplementInterfaceMethod(
        some_type
    );
  }
};

class ImplementedClass : public InterfaceName<ImplementedClass> {
public:
  ImplementedClass(int constructor_arg);
  SomeType ImplementInterfaceMethod(SomeType &some_instance);
  int data_member_;
};

ImplementedClass::ImplementedClass(int constructor_arg)
    : data_member_{constructor_arg} {}

SomeType ImplementedClass::ImplementInterfaceMethod(SomeType &some_instance) {
  auto new_value = some_instance.value + data_member_;
  return SomeType{new_value};
}


int main () {
  SomeType my_struct_instance{1};
  
  ImplementedClass concrete_class_instance{2};

  auto result = concrete_class_instance.InterfaceMethod(my_struct_instance);

  std::cout << result.value << std::endl;

  return 0;
}