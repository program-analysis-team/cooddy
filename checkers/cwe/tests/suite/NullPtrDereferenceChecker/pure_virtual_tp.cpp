
struct A {
    void func();
};

struct S {
    virtual A* getA() = 0;
};

void foo(S* s)
{
    s->getA()->func();  // NPD here
}
