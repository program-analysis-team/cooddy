struct A {
    int* x;
};

int** foo()
{
    A a;
    return &a.x;
}
