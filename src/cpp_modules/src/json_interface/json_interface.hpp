#ifndef DAB10F49_F5B4_46DD_963B_D40DE5483C20
#define DAB10F49_F5B4_46DD_963B_D40DE5483C20

#include <string>
#include <common.hpp>


namespace json_interface {

    class JsonIO {
public:
  virtual ~JsonIO() = default;

  virtual GamePointsSMap_t Import(string file_path) = 0; 
  virtual void Export(GamePointsSMap_t &data, string file_path) = 0;
  virtual bool Validate(string data_file, string schema_file) = 0;
};

}

#endif /* DAB10F49_F5B4_46DD_963B_D40DE5483C20 */
