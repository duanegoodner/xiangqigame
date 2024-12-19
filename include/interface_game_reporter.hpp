#pragma once

#include <game_data_structs.hpp>

class GameReporterInterface {
public:
  virtual void ReportGameInfo(const game::GameStatus &game_status) = 0;
  virtual ~GameReporterInterface() = default;
};
