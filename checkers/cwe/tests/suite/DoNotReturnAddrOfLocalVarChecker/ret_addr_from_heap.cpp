struct A {
    int** x;
};

void foo(int*** z)
{
    A a;
    a.x = (int**)42;
    *z = &a.x[0];
}
