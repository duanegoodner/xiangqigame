#include <iostream>
#include <string>
using namespace std;


template <class T> class MyClassTemplate {
    public:
    T evaluator;
    int stored_value;

    int DoEvaluation() {
        return evaluator.RunEvaluation(stored_value);
    }
};


class ConcreteEvaluator {
    public:
    int offset;
    int RunEvaluation(int x) {
        return offset + x;
    }
};


int main() {

    MyClassTemplate<ConcreteEvaluator> evaluator{5, 100};
    auto result = evaluator.DoEvaluation();

    cout << result << endl;

    return 0;

}


