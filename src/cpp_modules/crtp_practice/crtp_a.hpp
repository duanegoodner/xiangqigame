#include <cmath>
#include <string>
#include <vector>

using namespace std;


class Thing {
    public:
    string name;
    int val;
};

template<typename ConcreteRater, typename RatingType, typename ItemType>
class RaterInterface {
    public:
    RatingType RateThing(ItemType thing) {
        return static_cast<ConcreteRater*>(this)->ImplementRateThing(
            thing
        );
    }
};

template<typename ConcreteSelector, typename ItemType>
class SelectorInterface {
    public:
    ItemType SelectThing(
        vector<ItemType> &all_things) {
            return static_cast<ConcreteSelector*>(this)->ImplementSelectThing(
                all_things);
        }
};

template <typename ConcreteRater, typename RatingType, typename ItemType>
class SelectorInterfaceWithRater : public
    SelectorInterface< SelectorInterfaceWithRater <ConcreteRater, RatingType, ItemType>, ItemType> {
    public:
    vector<ItemType> best_things{};
    RaterInterface<ConcreteRater, RatingType, ItemType> evaluator_;
    
    ItemType ImplementSelectThing(vector<ItemType> &all_things); 

};