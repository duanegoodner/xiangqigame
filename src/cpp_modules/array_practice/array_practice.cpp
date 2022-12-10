#include <array>
#include <iostream>

#include "shared_components.hpp"

using namespace std;
using namespace board_components;


int main() {
    auto result = sizeof(int);

    auto my_supposed_index = PieceType::kGen + 1;

    cout << result << endl;

    return 0;
}