#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

class Person {
    public:
    void SayHello() {
        std::cout << "Hello there" << std::endl;
    }

};

template <typename E>
class EvaluatorBase {
    public:

    template <typename I>
    std::vector<int> GiveThingToWorkOn(I &&input_a, I &&input_b) {
        return static_cast<E*>(this)->ImplementGiveThingToWorkOn(
            std::forward<I>(input_a),
            std::forward<I>(input_b));
    }
};

class ImplementedEvaluator : public EvaluatorBase<ImplementedEvaluator> {
    public:
    ImplementedEvaluator(int divisor) : divisor_{divisor} {};
    std::vector<int> ImplementGiveThingToWorkOn(int input_a, int input_b) {
        std::vector<int> thing_to_work_on;
        thing_to_work_on.emplace_back(input_a % divisor_);
        thing_to_work_on.emplace_back(input_b % divisor_);
        return thing_to_work_on;
    }

    private:
    int divisor_;
};


template <typename S>
class SelectorBase {
    public:
    template<typename T>
    int SelectItem(
        T &&things) {
            return static_cast<S*>(this)->ImplementSelectItem(
                std::forward<T>(things));
        }
};


class FirstItemSelector : public SelectorBase<FirstItemSelector> {
    public:    
    int ImplementSelectItem(
        std::vector<int> things) {
            return things[0];
        }
};


class EvaluatedItemSelector : public SelectorBase<EvaluatedItemSelector> {
    
    public:
    ImplementedEvaluator evaluator_;
    EvaluatedItemSelector(ImplementedEvaluator evaluator)
    : evaluator_{evaluator} {};
    int ImplementSelectItem(std::vector<int> things) {
        auto transformed_things = evaluator_.GiveThingToWorkOn(
            things[0], things[1]);
        return transformed_things[1];
    }
};

int main() {
    auto my_evaluator = ImplementedEvaluator(3);
    auto my_selector = EvaluatedItemSelector(my_evaluator);

    std::vector<int> some_things = {43, 23, 9};

    auto selected_item = my_selector.ImplementSelectItem(some_things);

    return 0;
}
