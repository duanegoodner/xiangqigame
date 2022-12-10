template <template <typename> class Derived, typename T>
class base {
public:
    typedef T value_type;
    value_type foo() {
        return static_cast<Derived<T>*>(this)->foo();
    }
};

template <typename T>
class Derived : public base<Derived, T> {
public:
    typedef T value_type;
    value_type foo() {
        return T(); //return some T object (assumes T is default constructable)
    }
};

int main() {
    Derived<int> a;

    return 0;
}