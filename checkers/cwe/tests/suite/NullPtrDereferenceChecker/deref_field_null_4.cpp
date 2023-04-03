struct A {
    int* x;
};

struct B {
    A* a;
};

struct C {
    B* b[10];
};

struct D {
    C* c;
};

int foo(D* d)
{
    d->c->b[3]->a->x = nullptr;
    return *d->c->b[3]->a->x;
}
