struct A {
    virtual ~A() = default;
};

class B : public A {};

int main() {
    B* b = new B();
    A* a = b;
    delete b;
}
