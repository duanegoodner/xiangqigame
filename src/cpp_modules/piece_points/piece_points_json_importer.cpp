#include <piece_points.hpp>
#include <utility_functs.hpp>

using namespace std;
using namespace piece_points;
using json = nlohmann::json;

int main() {

    string file_path{"/home/duane/workspace/project/src/cpp_modules/piece_points/ICGA_2004.json"};

    auto imported_piece_points = utility_functs::import_json(file_path);
    auto result = PointsSpecExternal(imported_piece_points);

    auto result_2 = PointsSpecExternal(file_path);

    auto result_3 = PointsSpecInternal(result_2);

    return 0;
}

