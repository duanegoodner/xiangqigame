#include <iostream>
#include <string>
using namespace std;


template <typename Selector, class E> class AbstractSelector {
    public:
    E evaluator;
    int stored_value;

    int Evaluate() {
        return static_cast<Selector*>(this)->ImplementEvaluate();
    }
};


class ConcreteEvaluator {
    public:
    int RunEvaluation(int x) {
        return (2 * x);
    }
};

class ConcreteSelector : public AbstractSelector<ConcreteSelector, ConcreteEvaluator> {
    public:
    int ImplementEvaluate() {
        return evaluator.RunEvaluation(stored_value);
    }
};

class NullEvaluator {

};


class ConcreteSelectorWithoutEvaluator : public AbstractSelector<ConcreteSelectorWithoutEvaluator, NullEvaluator> {
    public:
    int ImplementEvaluate() {
        return 12;
    }
};


int main() {

    ConcreteEvaluator my_evaluator;
    auto my_selector = ConcreteSelector{my_evaluator, 5};
    auto result_a = my_selector.ImplementEvaluate(); 

    cout << result_a << endl;

    auto my_simple_selector = ConcreteSelectorWithoutEvaluator{NullEvaluator(), 5};
    auto result_b = my_simple_selector.ImplementEvaluate();
    cout << result_b << endl;

    return 0;

}


