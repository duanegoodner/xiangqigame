#pragma once


#include <board_data_structs.hpp>

class SpaceInfoProviderBase {
public:
  virtual ~SpaceInfoProviderBase() = default;
  virtual gameboard::BoardMap_t map() = 0;
};