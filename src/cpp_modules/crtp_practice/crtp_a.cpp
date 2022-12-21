#include <cmath>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

template <typename ValueType>
class Thing {
    public:
    string name;
    ValueType val;
};

template<typename ImplementedEvaluator, typename RatingType, typename ValueType>
class EvaluatorInterface {
    public:
    RatingType EvaluateThing(Thing<ValueType> thing) {
        return static_cast<ImplementedEvaluator*>(this)->ImplementEvaluateThing(
            thing
        );
    }
};

class AbsPlusOffsetEvaluator : public EvaluatorInterface<AbsPlusOffsetEvaluator, int, int> {
    public:
    int offset_;
    AbsPlusOffsetEvaluator(int offset) : offset_{offset} {};
    int ImplementEvaluateThing(Thing<int> thing) {
        return abs(thing.val) + offset_;
    }
};

template<typename ConcreteSelector, typename ValueType>
class AbstractSelector {
    public:
    Thing<ValueType> SelectThing(
        vector<Thing<ValueType>> &all_things) {
            return static_cast<ConcreteSelector*>(this)->ImplementSelectThing(
                all_things);
        }
};


template <typename ConcreteEvaluator, typename RatingType, typename ValueType>
class AbstractSelectorWithEvaluator : public AbstractSelector<AbstractSelectorWithEvaluator<ConcreteEvaluator, RatingType, ValueType>, ValueType> {
    public:
    vector<Thing<ValueType>> best_things{};
    EvaluatorInterface<ConcreteEvaluator, RatingType, ValueType> evaluator_;
    
    Thing<ValueType> ImplementSelectThing(vector<Thing<ValueType>>  &all_things) {    
        RatingType best_eval_result{};
        for (auto thing : all_things) {
            auto cur_eval_result = evaluator_.EvaluateThing(thing);
            if (cur_eval_result > best_eval_result) {
                best_things.clear();
                best_things.emplace_back(thing);
            } else if (cur_eval_result == best_eval_result) {
                best_things.emplace_back(thing);
            }
        }
        // in practice, have a tie-breaker selector instead of just choosing first element
        return best_things[0];
    }
};


class AbsPlusOffsetSelector : public AbstractSelectorWithEvaluator<AbsPlusOffsetEvaluator, int, int> {
    public:
    AbsPlusOffsetSelector(AbsPlusOffsetEvaluator evaluator)  {
        this->evaluator_ = evaluator;
    };
};


int main() {

    Thing<int> thing_a{"first", 1};
    Thing<int> thing_b{"second", 2};
    Thing<int> thing_c{"third", 3};

    vector<Thing<int>> my_things{thing_a, thing_b, thing_c};

    auto my_evaluator = AbsPlusOffsetEvaluator(5);
    auto my_selector = AbsPlusOffsetSelector(my_evaluator);
    auto result = my_selector.ImplementSelectThing(my_things);
       
    return 0;
}


