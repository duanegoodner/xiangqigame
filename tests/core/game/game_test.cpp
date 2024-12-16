#include <base_evaluator_factory.hpp>
#include <base_move_evaluator.hpp>
#include <base_space_info_provider.hpp>
#include <builders.hpp>
#include <game.hpp>
#include <game_board_for_concepts.hpp>
#include <game_factory.hpp>
#include <game_piece.hpp>
#include <gtest/gtest.h>
#include <integer_types.hpp>
#include <iostream>
#include <memory>
#include <move_evaluator_human_for_concepts.hpp>
#include <move_evaluator_minimax_for_concepts.hpp>
#include <move_evaluator_random_for_concepts.hpp>
#include <optional>
#include <piece_position_points_for_concepts.hpp>
#include <unordered_map>
#include <vector>
#include <zobrist_for_concepts.hpp>

// enum PlayerType : int { kHuman = 0, kRandom = 1, kMinimax = 3 };

// enum KeySizeBits : int { k032 = 0, k064 = 1, k128 = 2 };

// class MinimaxInfoBase {
// public:
//   virtual ~MinimaxInfoBase() = default;
// };

// template <typename K>
// class MinimaxInfo : MinimaxInfoBase {
// public:
//   using KeyType = K;
//   size_t num_conf_keys;
//   DepthType search_depth;
// };

// struct PlayerSpec {
//   PlayerType player_type;
//   gameboard::PieceColor color;
//   std::optional<KeySizeBits> key_size;
//   std::optional<const size_t> num_conf_keys;
//   std::optional<DepthType> search_depth;
//   std::optional<std::istream &> input_stream;
// };

class GameTest : public ::testing::Test {
protected:
  gameboard::GameBoardFactory game_board_factory_;
  std::shared_ptr<gameboard::GameBoardForConcepts> game_board_ =
      game_board_factory_.Create();
  DepthType default_search_depth_{3};

  //   std::shared_ptr<EvaluatorFactoryBase> GetEvaluatorFactory(const PlayerSpec
  //   &player_spec ) {
  //     if (player_spec.player_type == PlayerType::kHuman) {
  //       return std::make_shared<moveselection::HumanMoveEvaluatorFactory>(
  //           player_spec.input_stream
  //       );
  //     }
  //     if (player_spec.player_type == PlayerType::kRandom) {
  //       return std::make_shared<moveselection::RandomMoveEvaluatorFactory>();
  //     }
  //     if (player_spec.player_type == PlayerType::kMinimax) {
  //       if (player_spec.key_size == KeySizeBits::k032) {
  //         return std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<
  //             uint32_t,
  //             player_spec.num_conf_keys>>(game_board_, *player_spec.search_depth);
  //       }
  //       if (player_spec.key_size == KeySizeBits::k064) {
  //         return std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<
  //             uint64_t,
  //             player_spec.num_conf_keys>>(game_board_, *player_spec.search_depth);
  //       } else {
  //         return std::make_shared<moveselection::MinimaxMoveEvaluatorFactory<
  //             __uint128_t,
  //             player_spec.num_conf_keys>>(game_board_, *player_spec.search_depth);
  //       }
  //     }
  //   }
};

TEST_F(GameTest, InstantiateGame) {
  std::unordered_map<gameboard::PieceColor, std::unique_ptr<MoveEvaluatorBase>>
      evaluators;

  moveselection::MinimaxMoveEvaluatorFactory<uint64_t, 1> red_evaluator_factory{
      game_board_,
      default_search_depth_
  };
  moveselection::MinimaxMoveEvaluatorFactory<uint64_t, 1> black_evaluator_factory{
      game_board_,
      default_search_depth_
  };

  evaluators.emplace(
      gameboard::PieceColor::kRed,
      red_evaluator_factory.Create(gameboard::PieceColor::kRed)
  );
  evaluators.emplace(
      gameboard::PieceColor::kBlk,
      black_evaluator_factory.Create(gameboard::PieceColor::kBlk)
  );

  auto game = game::Game(game_board_, std::move(evaluators));

  game.Play();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}