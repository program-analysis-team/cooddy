struct A {
    int*a;
};

int someUndefinedRet();
A* someUndefinedRetStruct();

int foo(int* val) {
    int ret;
    auto paramA = someUndefinedRetStruct();
    while(true){
        if (*paramA->a == 123) {
            *val = 1;
            return 0;
        }
    }
    return 178;
}

int main()
{
    int a;
    auto ret = foo(&a);
    if (ret != 0) {
        return -1;
    }
    return a - 1;
}
