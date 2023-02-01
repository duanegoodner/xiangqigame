#ifndef C3B667BB_CCB8_4FB0_9563_B0F1A298F387
#define C3B667BB_CCB8_4FB0_9563_B0F1A298F387

#include <string>
#include <unordered_map>

namespace points_containers {

/*
struct w/ piece data members to map with piece name strings as keys
 */
template <typename ContainerType, typename PieceDataType>
unordered_map<string, PieceDataType> team_level_struct_to_map(ContainerType &container
) {
  unordered_map<string, PieceDataType> piece_values = {
      {"null", container.null},
      {"general", container.general},
      {"soldier", container.soldier},
      {"advisor", container.advisor},
      {"elephant", container.elephant},
      {"horse", container.horse},
      {"chariot", container.chariot},
      {"cannon", container.cannon}};
  return piece_values;
}

/*
map of piece-wise data to struct
 */
template <typename ContainerType, typename PieceDataType>
void set_piece_attributes(
    ContainerType &team_level_struct,
    unordered_map<string, PieceDataType> piece_member_map
) {
  // ContainerType team_level_struct{};
  team_level_struct.null = piece_member_map.at("null");
  team_level_struct.general = piece_member_map.at("general");
  team_level_struct.soldier = piece_member_map.at("soldier");
  team_level_struct.advisor = piece_member_map.at("advisor");
  team_level_struct.elephant = piece_member_map.at("elephant");
  team_level_struct.horse = piece_member_map.at("horse");
  team_level_struct.chariot = piece_member_map.at("chariot");
  team_level_struct.cannon = piece_member_map.at("cannon");
  // return team_level_struct;
}

/*
struct w/ color data members to map with color name strings as keys
 */
template <typename ContainerType, typename ColorDataType>
unordered_map<string, ColorDataType> color_member_map(ContainerType &container
) {
  unordered_map<string, ColorDataType> color_values = {
      {"red", container.red},
      {"black", container.black}};
  return color_values;
}

template <
    typename GameContainerType,
    typename TeamContainerType,
    typename PieceDataType>
void set_color_attributes(
    GameContainerType &game_level_struct,
    unordered_map<string, unordered_map<string, PieceDataType>> color_member_map
) {
  TeamContainerType red{};
  set_piece_attributes<TeamContainerType, PieceDataType>(
    red, color_member_map.at("red"));
  
  TeamContainerType black{};
  set_piece_attributes<TeamContainerType, PieceDataType>(
    black, color_member_map.at("black"));

  game_level_struct.red = red;
  game_level_struct.black = black;
}


}


#endif /* C3B667BB_CCB8_4FB0_9563_B0F1A298F387 */
