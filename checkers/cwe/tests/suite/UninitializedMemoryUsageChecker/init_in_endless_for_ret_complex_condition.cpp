struct A {
    int* a;
};

A* someUndefinedRetStruct();
int someRet();

int foo(int* val)
{
    auto paramA = someUndefinedRetStruct();
    int ret;
    for (;;) {
        ret = someRet();
        if(ret!=0) {
            return ret;
        }
        if (*paramA->a == 123) {
            *val = 1;
            return 0;
        }
    }
    return 1;
}

int boo(int val)
{
    int a;

    auto ret = foo(&a);
    if (ret != 0) {
        return 0;
    }
    return a - 1;
}
