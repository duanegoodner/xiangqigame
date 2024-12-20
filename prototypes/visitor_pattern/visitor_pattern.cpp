#include <iostream>
#include <memory>

class MyInterface {
public:
    virtual ~MyInterface() = default;
    virtual void MyMethod() = 0;
    virtual void Accept(class Visitor& visitor) = 0;
};

class Visitor {
public:
    virtual void VisitSimple(class MyConcreteClassSimple& simple) = 0;
    virtual void VisitComplex(class MyConcreteClassComplex& complex) = 0;
};

class MyConcreteClassSimple : public MyInterface {
public:
    void MyMethod() override {
        std::cout << "Simple Implementation" << std::endl;
    }
    void Accept(Visitor& visitor) override {
        visitor.VisitSimple(*this);
    }
};

class MyConcreteClassComplex : public MyInterface {
public:
    void MyMethod() override {
        std::cout << "Complex Implementation" << std::endl;
    }
    void AdditionalMethod() {
        std::cout << "Additional Method in Complex Class" << std::endl;
    }
    void Accept(Visitor& visitor) override {
        visitor.VisitComplex(*this);
    }
};

class ConcreteVisitor : public Visitor {
public:
    void VisitSimple(MyConcreteClassSimple& simple) override {
        std::cout << "Visiting Simple Class" << std::endl;
    }
    void VisitComplex(MyConcreteClassComplex& complex) override {
        std::cout << "Visiting Complex Class" << std::endl;
        complex.AdditionalMethod();
    }
};

void consume(std::shared_ptr<MyInterface> obj) {
    ConcreteVisitor visitor;
    obj->Accept(visitor);
}

int main() {
    auto simpleObj = std::make_shared<MyConcreteClassSimple>();
    auto complexObj = std::make_shared<MyConcreteClassComplex>();
    
    consume(simpleObj);
    consume(complexObj);
    
    return 0;
}
