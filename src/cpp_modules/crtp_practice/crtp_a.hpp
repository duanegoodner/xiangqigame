#include <cmath>
#include <vector>

using namespace std;


template <typename ValueType>
class Thing {
    public:
    string name;
    ValueType val;
};

template<typename ConcreteRater, typename RatingType, typename ValueType>
class RaterInterface {
    public:
    RatingType RateThing(Thing<ValueType> thing) {
        return static_cast<ConcreteRater*>(this)->ImplementRateThing(
            thing
        );
    }
};

template<typename ConcreteSelector, typename ValueType>
class SelectorInterface {
    public:
    Thing<ValueType> SelectThing(
        vector<Thing<ValueType>> &all_things) {
            return static_cast<ConcreteSelector*>(this)->ImplementSelectThing(
                all_things);
        }
};

template <typename ConcreteRater, typename RatingType, typename ValueType>
class SelectorInterfaceWithRater : public
    SelectorInterface< SelectorInterfaceWithRater <ConcreteRater, RatingType, ValueType>, ValueType> {
    public:
    vector<Thing<ValueType>> best_things{};
    RaterInterface<ConcreteRater, RatingType, ValueType> evaluator_;
    
    Thing<ValueType> ImplementSelectThing(vector<Thing<ValueType>>  &all_things); 

};