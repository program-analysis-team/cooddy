struct A {
    int*a;
};

int someUndefinedRet();
A* someUndefinedRetStruct();

int foo(int* val) {
    int ret;
    auto paramA = someUndefinedRetStruct();
    for(;false;){
        ret = someUndefinedRet();
        if ( ret != 0) {
            return ret;
        }
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
    return a - 1;
}
