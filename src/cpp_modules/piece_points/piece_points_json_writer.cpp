#include "common.hpp"
#include <piece_points.hpp>
#include <utility_functs.hpp>

using namespace std;
using namespace piece_points;
using json = nlohmann::json;

int main() {

  PointsSpecExternal ICGA2004{
      kBasePointsMapICGA2004,
      kNullBasePointsOffsetMap,
      kPositionPointsMapICGA2004,
      kNullPositionPointsOffsetMap};
  auto ICGA2004_json = ICGA2004.ToJson();
  utility_functs::export_json(ICGA2004_json, "ICGA_2004.json");

  return 0;
}