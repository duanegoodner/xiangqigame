#ifndef CD16B57C_0103_4E3D_88B2_00B7F36097E4
#define CD16B57C_0103_4E3D_88B2_00B7F36097E4

#include <hash_calculator.hpp>
#include <string>
#include <utility_functs.hpp>

using namespace utility_functs;

const string DEFAULT_ZKEYS_FILEPATH = get_data_file_abs_path("zkeys_v01.json");
zkey_t random_zkey();
game_zarray_t create_zarray();

#endif /* CD16B57C_0103_4E3D_88B2_00B7F36097E4 */
