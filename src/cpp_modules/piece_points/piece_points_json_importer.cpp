#include "common.hpp"
#include <piece_points.hpp>
#include <utility_functs.hpp>

using namespace std;
using namespace piece_points;
using json = nlohmann::json;

int main() {

    string file_path{"/home/duane/workspace/project/src/cpp_modules/piece_points/ICGA_2004.json"};

    auto imported_piece_points = utility_functs::import_json(file_path);
    // auto result = PointsSpecExternal(imported_piece_points);

    auto external_points_spec = PointsSpecExternal(file_path);

    auto internal_points_spec = PointsSpecInternal(external_points_spec);

    auto new_builder = PiecePointsBuilder(internal_points_spec);

    auto new_game_points = new_builder.BuildGamePositionPoints();

    GamePositionPoints_t old_game_points = DEFAULT_GAME_POINTS;

    return 0;
}

