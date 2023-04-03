struct A {
    virtual ~A() = default;
};

struct B : public A {};

int main() {
    A* a = new B();
    B* staticB = static_cast<B*>(a);
    B* dynamicB = dynamic_cast<B*>(a);
    delete a;
}
