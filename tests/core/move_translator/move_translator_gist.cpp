#include <move_translator.hpp>

int main() {

  std::string input{"a1, b9, d5, q22"};
  auto tokens = movetranslation::Tokenize(input);
  auto algebraic_board_spaces =
      movetranslation::ConvertTokensToAlgebraicBoardSpaces(tokens);

  std::cout << "The AlgebraicBoardSpace values are:" << std::endl;
  for (auto &space : algebraic_board_spaces) {
    std::cout << space.value() << std::endl;
  }

  return 0;
}