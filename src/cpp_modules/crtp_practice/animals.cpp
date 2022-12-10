#include <iostream>

using namespace std;

template<typename specific_animal>
struct animal {
    int add_something(int my_num) { static_cast<specific_animal*>(this)->add_something(int my_num); }
};

struct dog : animal<dog> {
    int add_something(int my_num) { 
        return my_num + 1;
        }
};

struct cat : animal<cat> {
    int add_something(int my_num) { 
        return my_num + 2; 
        }
};

template<typename specific_animal>
void add_it(animal<specific_animal> &animal, int my_num) {
    animal.add_something(my_num);
}

int main() {
    cat c;
    add_it(c, 1); // prints `cat`

    dog d;
    add_it(d, 1); // prints `dog`

    return 0;
}

