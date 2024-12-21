#pragma once

#include <moveselection/evaluator_data_structs.hpp>
#include <gameboard/move_data_structs.hpp>
#include <optional>

class MoveEvaluatorBase {
public:
  virtual gameboard::Move SelectMove(const gameboard::MoveCollection &allowed_moves) = 0;
  virtual void NotifyIllegalMove() = 0;
  virtual const std::optional<moveselection::SearchSummaries> search_summaries() const {
    return std::nullopt;
  }
  virtual ~MoveEvaluatorBase() = default;
};