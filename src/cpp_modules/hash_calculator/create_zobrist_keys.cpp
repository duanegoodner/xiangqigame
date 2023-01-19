
#include <array>
#include <bits/stdc++.h>
#include <board_components.hpp>
#include <fstream>
#include <hash_calculator.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <utility_functs.hpp>

using namespace board_components;
using json = nlohmann::json;
using namespace std;

int main(int argc, char *argv[]) {

    auto new_turn_key = random_zkey();
    auto new_zarray = create_zarray();
    auto new_zorbist_keys = ZobristKeys(new_turn_key, new_zarray);
    json json_object = new_zorbist_keys.ToJson();
    utility_functs::export_json(json_object, argv[1]);

    return 0;
}
