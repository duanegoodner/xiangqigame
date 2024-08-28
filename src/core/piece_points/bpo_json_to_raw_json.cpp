#include <piece_points.hpp>
#include <piece_points_spec.hpp>

using namespace piece_points;
using namespace piece_points_spec;

int main(int argc, char **argv) {

  const string bpo_path = argv[1];
  const string output_path = argv[2];

  auto game_points_array =
      GamePointsArrayBuilder(bpo_path).BuildGamePointsArray();
  auto piece_points = PiecePoints(game_points_array);
  piece_points.ToFile(output_path);

  return 0;
}