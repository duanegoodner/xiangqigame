#include <iostream>
#include <vector>
#include <algorithm> // for std::find_if

struct MyStruct {
    int id;
    std::string name;
};

int main() {
    std::vector<MyStruct> vec = {
        {1, "Alice"},
        {2, "Bob"},
        {3, "Charlie"}
    };

    int id_to_find = 2;

    // Using std::find_if to find the struct with a specific id
    auto it = std::find_if(vec.begin(), vec.end(), [id_to_find](const MyStruct& s) {
        return s.id == id_to_find;
    });

    if (it != vec.end()) {
        std::cout << "Struct with ID " << id_to_find << " found. Name: " << it->name << '\n';
    } else {
        std::cout << "Struct with ID " << id_to_find << " not found.\n";
    }

    return 0;
}