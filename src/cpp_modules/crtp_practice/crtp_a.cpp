#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "crtp_a.hpp"

using namespace std;


template <typename ConcreteRater, typename RatingType, typename ValueType>
Thing<ValueType> SelectorInterfaceWithRater<ConcreteRater, RatingType, ValueType>::ImplementSelectThing(vector<Thing<ValueType>>  &all_things)  {    
        RatingType best_eval_result{};
        for (auto thing : all_things) {
            auto cur_eval_result = evaluator_.RateThing(thing);
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


class AbsPlusOffsetRater : public RaterInterface<AbsPlusOffsetRater, int, int> {
    public:
    int offset_;
    AbsPlusOffsetRater(int offset) : offset_{offset} {};
    int ImplementRateThing(Thing<int> thing) {
        return abs(thing.val) + offset_;
    }
};


class AbsPlusOffsetSelector : public SelectorInterfaceWithRater<AbsPlusOffsetRater, int, int> {
    public:
    AbsPlusOffsetSelector(AbsPlusOffsetRater evaluator)  {
        this->evaluator_ = evaluator;
    };
};


int main() {

    Thing<int> thing_a{"first", 1};
    Thing<int> thing_b{"second", 2};
    Thing<int> thing_c{"third", 3};

    vector<Thing<int>> my_things{thing_a, thing_b, thing_c};

    auto my_evaluator = AbsPlusOffsetRater(5);
    auto my_selector = AbsPlusOffsetSelector(my_evaluator);
    auto result = my_selector.SelectThing(my_things);
       
    return 0;
}


