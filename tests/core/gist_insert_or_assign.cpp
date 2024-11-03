#include <iostream>
#include <unordered_map>
#include <string>

struct MyStruct {
    std::string name;
    int age;
};

int main() {
    std::unordered_map<int, MyStruct> map;

    // Using insert_or_assign to add a new key
    auto result = map.insert_or_assign(1, MyStruct{"John Doe", 30});
    std::cout << "Inserted: " << result.second << ", Name: " << result.first->second.name << ", Age: " << result.first->second.age << std::endl;

    // Using insert_or_assign to update an existing key
    result = map.insert_or_assign(1, MyStruct{"Jane Doe", 25});
    std::cout << "Updated: " << !result.second << ", Name: " << result.first->second.name << ", Age: " << result.first->second.age << std::endl;

    // Display the current state of the map
    for (const auto& [key, value] : map) {
        std::cout << "Key: " << key << ", Name: " << value.name << ", Age: " << value.age << std::endl;
    }

    return 0;
}
