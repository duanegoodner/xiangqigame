#pragma once

#include <base_move_evaluator.hpp>
#include <base_space_info_provider.hpp>
#include <concept_composite_concepts.hpp>
#include <concept_move_evaluator.hpp>
#include <game.hpp>


class GameFactory {
    game::Game Create();
};