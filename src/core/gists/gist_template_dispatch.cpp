#include <iostream>
#include <type_traits>

typedef uint64_t my_key_type;

namespace TypeHandler {
    // Generic template method (will cause a compile-time error if used)
    template <typename T>
    void handle_type() {
        static_assert(sizeof(T) == 0, "Type not supported!");
    }

    // Specialization for uint64_t
    template <>
    void handle_type<uint64_t>() {
        std::cout << "my_key_type is uint64_t\n";
    }

    // Specialization for __uint128_t
    template <>
    void handle_type<__uint128_t>() {
        std::cout << "my_key_type is __uint128_t\n";
    }

    // Function to check the type and call the appropriate method
    void check_type() {
        handle_type<my_key_type>();
    }
}

int main() {
    TypeHandler::check_type();
    return 0;
}
