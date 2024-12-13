#include <template_factory.hpp>
#include <iostream>
#include <string>

int main() {
    std::string userInput;
    std::cout << "Enter type (int, float, string): ";
    std::cin >> userInput;

    TemplateFactory template_facory_;
    
    if (userInput == "int") {
        auto instance = template_facory_.getInstance<int>("example");
        instance->doSomething();
    } else if (userInput == "float") {
        auto instance = template_facory_.getInstance<float>("example");
        instance->doSomething();
    } else if (userInput == "string") {
        auto instance = template_facory_.getInstance<std::string>("example");
        instance->doSomething();
    } else {
        std::cout << "Unsupported type!" << std::endl;
    }

    return 0;
}
