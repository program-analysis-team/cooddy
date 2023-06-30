struct B {
    int *x;
    int *xx;
    int *i;
};

struct A {
    int *x;
    int *xx;
    int *xxx;
    B* b;
};

int foo(A* a)
{
    a->b->i = nullptr;
    return *a->b->i;
}
