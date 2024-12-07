#include <memory>
#include <variant>
#include <vector>
#include <iostream>

struct Event {};

// Example calculators
struct CalculatorA {
    void UpdateState(const Event&) {
        std::cout << "CalculatorA UpdateState\n";
    }
};

struct CalculatorB {
    void UpdateState(const Event&) {
        std::cout << "CalculatorB UpdateState\n";
    }
};

// Type alias for a variant holding shared pointers to different calculator types
using CalculatorVariant = std::variant<
    std::shared_ptr<CalculatorA>,
    std::shared_ptr<CalculatorB>>;

class InfoTracker {
    std::vector<CalculatorVariant> calculators_a_;
    std::vector<CalculatorVariant> calculators_b_;

public:
    // Add a shared pointer to vector a
    template <typename T>
    void AddCalculatorA(std::shared_ptr<T> calculator) {
        calculators_a_.emplace_back(std::move(calculator));
    }

    // Add a shared pointer to vector b
    template <typename T>
    void AddCalculatorB(std::shared_ptr<T> calculator) {
        calculators_b_.emplace_back(std::move(calculator));
    }

    // Update all calculators in vector a
    void UpdateAllA(const Event& event) {
        for (const auto& calculator : calculators_a_) {
            std::visit([&event](const auto& calc) {
                calc->UpdateState(event);
            }, calculator);
        }
    }

    // Update all calculators in vector b
    void UpdateAllB(const Event& event) {
        for (const auto& calculator : calculators_b_) {
            std::visit([&event](const auto& calc) {
                calc->UpdateState(event);
            }, calculator);
        }
    }
};

int main() {
    Event event;

    auto calcA = std::make_shared<CalculatorA>();
    auto calcB = std::make_shared<CalculatorB>();

    InfoTracker tracker;
    tracker.AddCalculatorA(calcA);
    tracker.AddCalculatorB(calcB);

    tracker.UpdateAllA(event);
    tracker.UpdateAllB(event);
}
