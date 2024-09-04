#include <gist_key_generator.hpp>
#include <random>


int main() {
     std::random_device rd;
     std::mt19937_64 gen_64(rd());

     my_key_type result = KeyGenerator::generate_key<my_key_type>(gen_64);
     KeyGenerator::display_key(result);

     return 0;   

}