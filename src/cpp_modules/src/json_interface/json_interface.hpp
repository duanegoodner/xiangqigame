// Filename: game_board.tpp
// Author: Duane Goodner
// Created: 2022-12-22
// Last Modified: 2024-08-16

// Description:
// Defines JsonIO interface which any class that reads/writes json must comply with.

#ifndef DAB10F49_F5B4_46DD_963B_D40DE5483C20
#define DAB10F49_F5B4_46DD_963B_D40DE5483C20

#include <string>
#include <common.hpp>


namespace json_interface {

    class JsonIO {
public:
  virtual ~JsonIO() = default;

  virtual void Import(GamePointsSMap_t& s_map, string file_path) = 0;
  virtual void Export(GamePointsSMap_t& data, string file_path) = 0;
  virtual void Import(BPOSpecSMap_t& s_map, string file_path) = 0;
  virtual void Export(BPOSpecSMap_t& data, string file_path) = 0;
  virtual bool Validate(string data_file, string schema_file) = 0;
};

}

#endif /* DAB10F49_F5B4_46DD_963B_D40DE5483C20 */
