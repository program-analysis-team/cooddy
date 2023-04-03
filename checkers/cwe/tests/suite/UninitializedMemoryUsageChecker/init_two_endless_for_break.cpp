struct A {
    int* a;
};

A* someUndefinedRetStruct();

void foo(int* val, int& flag)
{
    auto paramA = someUndefinedRetStruct();
    for (;;) {
        if (flag != 0) {
            break;
        }
        if (*paramA->a == 123) {
            *val = 1;
            return;
        }
    }

    for (;;) {
        if (flag == 41) {
            return;
        }
    }
}

int boo(int val)
{
    int a;

    foo(&a, val);
    if (val != 0) {
        return 0;
    }
    return a - 1;
}
