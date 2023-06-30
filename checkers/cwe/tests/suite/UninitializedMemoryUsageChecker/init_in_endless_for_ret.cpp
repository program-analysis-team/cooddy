struct A {
    int* a;
};

A* someUndefinedRetStruct();

int foo(int* val)
{
    auto paramA = someUndefinedRetStruct();
    for (;true;) {
        if (*paramA->a == 123) {
            *val = 1;
            return 0;
        }
    }
    return 1;
}

int boo()
{
    int a;

    auto ret = foo(&a);
    if (ret != 0) {
        return 0;
    }
    return a - 1;
}
