#pragma once

#include <base_move_evaluator.hpp>
#include <memory>

class EvaluatorFactoryBase {
    public:
    virtual std::shared_ptr<MoveEvaluatorBase> Create() = 0;
    virtual ~EvaluatorFactoryBase() = default;
};