#pragma once

#include <concept_board_state_coordinator.hpp>
#include <concept_composite_concepts.hpp>
#include <concept_move_evaluator.hpp>

namespace game {

template <
    SpaceInfoProviderAndCalculatorRegistryConcept G,
    MoveEvaluatorConcept RedEvaluator,
    MoveEvaluatorConcept BlackEvaluator>
class Game {};

} // namespace game