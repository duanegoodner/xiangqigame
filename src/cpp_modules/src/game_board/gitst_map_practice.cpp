#include <iostream>
#include <typeinfo>
#include <map>

int main() {
    std::map<int, int> myMap;

    myMap[0] = 1;
    myMap[3] = 0;
    myMap[4] = 0;
    myMap[0] = 2;
    
    auto it_a = myMap.find(0);
    auto it_b = myMap.find(4);
    auto it_c = myMap.find(5);
    auto it_d = myMap.find(4);

    // Accessing and printing the .first and .second of it_a
    if (it_a != myMap.end()) {
        std::cout << "it_a: key = " << it_a->first << ", value = " << it_a->second << std::endl;
    } else {
        std::cout << "it_a: Key not found." << std::endl;
    }

    // Accessing and printing the .first and .second of it_b
    if (it_b != myMap.end()) {
        std::cout << "it_b: key = " << it_b->first << ", value = " << it_b->second << std::endl;
    } else {
        std::cout << "it_b: Key not found." << std::endl;
    }

    // Accessing and printing the .first and .second of it_c
    if (it_c != myMap.end()) {
        std::cout << "it_c: key = " << it_c->first << ", value = " << it_c->second << std::endl;
    } else {
        std::cout << "it_c: Key not found." << std::endl;
    }
   
    
    return 0;
}
