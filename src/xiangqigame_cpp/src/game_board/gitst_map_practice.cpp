#include <iostream>
#include <map>

struct MyValueType {
    bool present;
    int value;
};

int main() {
    std::map<int, MyValueType> myMap;
    
    MyValueType my_item{true, 2};
    myMap[5] = my_item;
    
    auto it = myMap.find(5);
    
    if (it != myMap.end()) {
        std::cout << "Key 5 found - present: " << it->second.present << ", value: " << it->second.value << std::endl;
    } else {
        std::cout << "Key 5 not found in the map." << std::endl;
    }
    
    return 0;
}
