#include <cmath>
#include <string>
#include <vector>

using namespace std;


class Thing {
    public:
    string name;
    int val;
};

template<typename ConcreteRater>
class RaterInterface {
    public:
    int RateThing(Thing thing) {
        return static_cast<ConcreteRater*>(this)->ImplementRateThing(
            thing
        );
    }
};

class AbsPlusOffsetRater : public RaterInterface<AbsPlusOffsetRater> {
    public:
    int offset_;
    AbsPlusOffsetRater(int offset);
    int ImplementRateThing(Thing thing);
};

template<typename ConcreteSelector>
class SelectorInterface {
    public:
    Thing SelectThing(
        vector<Thing> &all_things) {
            return static_cast<ConcreteSelector*>(this)->ImplementSelectThing(
                all_things);
        }
};

template <typename ConcreteRater>
class SelectorInterfaceWithRater {
    public:
    vector<Thing> best_items_;
    RaterInterface<ConcreteRater> rater_;
    Thing ImplementSelectThing(vector<Thing> &all_things);
};

class AbsPlusOffsetSelector : public SelectorInterfaceWithRater<AbsPlusOffsetRater> {
    public:
    AbsPlusOffsetSelector(AbsPlusOffsetRater rater);

};
