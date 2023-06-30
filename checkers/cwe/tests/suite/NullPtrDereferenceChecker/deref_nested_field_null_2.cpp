struct B {
    int* i;
};

struct A {
    B* b[5];
};

int foo(A* a)
{
    a->b[2]->i = nullptr;
    return *a->b[2]->i;
}
