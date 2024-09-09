#include <chrono>
#include <game_board.hpp>
#include <hash_calculator.hpp>
#include <iostream>

using namespace std::chrono;

int main() {
  auto my_game_board =
      NewGameBoard();

  auto start = high_resolution_clock::now();

  for (auto idx = 0; idx < 100; idx++) {
    my_game_board.CalcFinalMovesOf(PieceColor::kRed);
  }

  auto end = high_resolution_clock::now();

  auto result = duration_cast<milliseconds>(end - start);

  std::cout << result.count() << std::endl;

  return 0;
}
