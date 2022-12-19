#include <iostream>
#include <string>


using namespace std;

template<class T> class TestTemplate {
public:
    //constructor 1
    template<class Y> TestTemplate(Y * p) {
        cout << "c1" << endl;
    }

    //constructor 2
    template<class Y, class D> TestTemplate(Y * p, D d) {
        cout << "c2" << endl;
    }

    template<class A, class B>
    void foo(A a, B b) {
        cout << "foo" << endl;
    }
};

int main() {

    TestTemplate<int> tp(new int());

    tp.foo<int*, string>(new int(), "hello");

    TestTemplate<int> tp2(new int(),2);

    tp2.foo<int*, string>(new int(), "hello_again");

    cout << "done" << endl;

}