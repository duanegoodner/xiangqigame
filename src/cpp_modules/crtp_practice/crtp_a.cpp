#include <cmath>
#include <iostream>
#include <string>
#include <vector>


using namespace std;


class Thing {
    public:
    string name;
    int val;
};

template<typename ImplementedEvaluator>
class EvaluatorInterface {
    public:
    int EvaluateThing(Thing thing) {
        return static_cast<ImplementedEvaluator*>(this)->ImplementEvaluateThing(
            thing
        );
    }
};

class AbsPlusOffsetEvaluator : public EvaluatorInterface<AbsPlusOffsetEvaluator> {
    public:
    int offset_;
    AbsPlusOffsetEvaluator(int offset) : offset_{offset} {};
    int ImplementEvaluateThing(Thing thing) {
        return abs(thing.val) + offset_;
    }
};

template<typename ConcreteSelector>
class AbstractSelector {
    public:
    Thing SelectThing(
        vector<Thing> &all_things) {
            return static_cast<ConcreteSelector*>(this)->ImplementSelectThing(
                all_things);
        }
};


template <typename ConcreteEvaluator>
class AbstractSelectorWithEvaluator : public AbstractSelector<AbstractSelectorWithEvaluator<ConcreteEvaluator>> {
    public:
    EvaluatorInterface<ConcreteEvaluator> evaluator_;
    Thing ImplementSelectThing(vector<Thing>  &all_things) {
        // return all_things[0];
        Thing selected_thing;
        int max_eval = 0;
        for (auto thing : all_things) {
            auto cur_eval = evaluator_.EvaluateThing(thing);
            if (cur_eval > max_eval) {
                max_eval = cur_eval;
                selected_thing = thing;
            }
        }
        return selected_thing;
    }
};


class AbsPlusOffsetSelector : public AbstractSelectorWithEvaluator<AbsPlusOffsetEvaluator> {
    public:
    AbsPlusOffsetSelector(AbsPlusOffsetEvaluator evaluator)  {
        this->evaluator_ = evaluator;
    };
};


int main() {

    Thing thing_a{"first", 1};
    Thing thing_b{"second", 2};
    Thing thing_c{"third", 3};

    vector<Thing> my_things{thing_a, thing_b, thing_c};

    auto my_evaluator = AbsPlusOffsetEvaluator(5);
    auto my_selector = AbsPlusOffsetSelector(my_evaluator);
    auto result = my_selector.ImplementSelectThing(my_things);
       
    return 0;
}


