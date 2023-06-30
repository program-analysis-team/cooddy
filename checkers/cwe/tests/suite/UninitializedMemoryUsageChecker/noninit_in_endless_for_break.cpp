struct A {
    int*a;
};

int someUndefinedRet();
A* someUndefinedRetStruct();

int foo(int* val) {
    int ret;
    auto paramA = someUndefinedRetStruct();
    for(;;){
        break;
        *val = 1;
    }
    return 178;
}

int main()
{
    int a;
    auto ret = foo(&a);
    return a - 1;
}
