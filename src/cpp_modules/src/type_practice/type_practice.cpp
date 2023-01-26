#include <array>
#include <iostream>
#include <typeinfo>

using namespace std;


struct Thing {
    int a;
    int b;
};


typedef array< array <Thing, 5>, 6> my_array_t;

int main() {

    my_array_t new_array{};

    const type_info& my_type = typeid(new_array);
    cout << my_type.hash_code() << endl;

    return 0;
}
