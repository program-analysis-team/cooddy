struct A {
    int x;
};

int* foo()
{
    A* a = (A*)42;
    return &a->x;
}
