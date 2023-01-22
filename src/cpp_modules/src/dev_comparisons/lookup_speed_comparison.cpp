#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include "../nlohmann_json/json.hpp"
#include "../utility_functs/utility_functs.hpp"
#include "../board_components/board_components.hpp"

using namespace board_components;
using namespace std::chrono;
using namespace nlohmann::json_abi_v3_11_2;
using namespace std;

typedef array<array<array<array<unsigned long long, kNumFiles>, kNumRanks>, kNumPieceTypeVals>, 2> zarray4d_t;
typedef array<unsigned long long, 2 * kNumRanks * kNumFiles *(kNumPieceTypeVals - 1)> zarray1d_t;

void populate_4d_null_vals(zarray4d_t &zarray4d)
{
    for (auto row = 0; row < kNumRanks; row++)
    {
        for (auto col = 0; col < kNumFiles; col++)
        {
            zarray4d[0][0][row][col] = (unsigned long long)0;
            zarray4d[1][0][row][col] = (unsigned long long)0;
        }
    }
}

void populate_4d_std_vals(zarray4d_t &zarray4d)
{
    for (auto piece_id = 1; piece_id < kNumPieceTypeVals; piece_id++)
    {
        for (auto row = 0; row < kNumRanks; row++)
        {
            for (auto file = 0; file < kNumFiles; file++)
            {
                zarray4d[0][piece_id][row][file] = utility_functs::random(
                    (unsigned long long)0, (unsigned long long)numeric_limits<unsigned long long>::max);
            }
        }
    }
}

void populate_1d_vals(zarray1d_t zarray1d)
{
    for (auto idx = 0; idx < zarray1d.size(); idx++)
    {
        zarray1d[idx] = utility_functs::random(
            (unsigned long long)0,
            (unsigned long long)numeric_limits<unsigned long long>::max);
    }
}

const int num_entries_per_color = 2 * kNumRanks * kNumFiles * (kNumPieceTypeVals - 1) / 2;

const vector<int> color_vals{-1, 1};
const vector<int> piece_type_vals{1, 2, 3, 4, 5, 6, 7};
const vector<int> rank_vals{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
const vector<int> file_vals{0, 1, 2, 3, 4, 5, 6, 7, 8};

int get_random_item_from(vector<int> id_vals)
{
    auto idx = utility_functs::random(int(0), (int)(id_vals.size() - 1));
    return id_vals[idx];
}

struct SpecificPieceAndSpace
{
    int color_id;
    int type_id;
    int rank;
    int file;
};

SpecificPieceAndSpace random_piece_and_space()
{
    SpecificPieceAndSpace piece_and_space{
        get_random_item_from(color_vals),
        get_random_item_from(piece_type_vals),
        get_random_item_from(rank_vals),
        get_random_item_from(file_vals)};

    return piece_and_space;
}

unsigned long long get_item_from_1darray(zarray1d_t zarray1d, SpecificPieceAndSpace piece_and_space)
{
    auto idx = piece_and_space.color_id * num_entries_per_color +
               (piece_and_space.type_id - 1) * kNumRanks * kNumFiles +
               piece_and_space.rank * kNumFiles +
               piece_and_space.file;
    return zarray1d[idx];
}

unsigned long long get_item_from_4darray(zarray4d_t zarray4d, SpecificPieceAndSpace piece_and_space)
{
    return zarray4d[piece_and_space.color_id][piece_and_space.type_id][piece_and_space.rank][piece_and_space.file];
}

int main()
{

    zarray4d_t zarray4d{0};
    zarray1d_t zarray1d{0};

    populate_4d_null_vals(zarray4d);
    populate_4d_std_vals(zarray4d);
    populate_1d_vals(zarray1d);

    // vector<SpecificPieceAndSpace> located_pieces;

    array<SpecificPieceAndSpace, 500000> located_pieces;
    for (auto idx = 0; idx < 500000; idx++)
    {
        // located_pieces.emplace_back(random_piece_and_space());
        located_pieces[idx] = random_piece_and_space();
    }

    // auto start_1d = high_resolution_clock::now();
    // for (auto item : located_pieces)
    // {
    //     auto hash_val_1d = get_item_from_1darray(zarray1d, item);
    // }
    // auto end_1d = high_resolution_clock::now();
    // auto time_1d = duration_cast<nanoseconds>(end_1d - start_1d);

    auto start_4d = high_resolution_clock::now();
    for (auto item : located_pieces)
    {
        auto hash_val_4d = get_item_from_4darray(zarray4d, item);
    }
    auto end_4d = high_resolution_clock::now();
    auto time_4d = duration_cast<nanoseconds>(end_4d - start_4d);

    // cout << "time 1d: " << time_1d.count() << endl;
    cout << "time 4d: " << time_4d.count() << endl;

    return 0;
}