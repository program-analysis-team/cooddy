struct A {
    int* x;
};

int* finish(A* a)
{
    return a->x;
}

int* foo()
{
    A a;
    return finish(&a);
}
