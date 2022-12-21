#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "crtp_b.hpp"

using namespace std;


AbsPlusOffsetRater::AbsPlusOffsetRater(int offset) : offset_{offset} {};
int AbsPlusOffsetRater::ImplementRateThing(Thing thing)
{
    return abs(thing.val) + offset_;
}

template <typename ConcreteRater>
Thing SelectorInterfaceWithRater<ConcreteRater>::ImplementSelectThing(
    vector<Thing> &all_things)
{
    vector<Thing> best_things{};
    int best_eval_result{};
    for (auto thing : all_things)
    {
        auto cur_eval_result = rater_.RateThing(thing);
        if (cur_eval_result > best_eval_result)
        {
            best_things.clear();
            best_things.emplace_back(thing);
        }
        else if (cur_eval_result == best_eval_result)
        {
            best_things.emplace_back(thing);
        }
    }
    return best_things[0];
}


     AbsPlusOffsetSelector::AbsPlusOffsetSelector(AbsPlusOffsetRater rater)
    {
        this->rater_ = rater;
    }

int main()
{

    Thing thing_a{"first", 1};
    Thing thing_b{"second", 2};
    Thing thing_c{"third", 3};

    vector<Thing> my_things{thing_a, thing_b, thing_c};

    auto my_evaluator = AbsPlusOffsetRater(5);
    auto my_selector = AbsPlusOffsetSelector(my_evaluator);
    auto result = my_selector.ImplementSelectThing(my_things);

    return 0;
}
