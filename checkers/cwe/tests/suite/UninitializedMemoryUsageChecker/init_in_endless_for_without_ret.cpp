struct A {
    int*a;
};

int someUndefinedRet();
A* someUndefinedRetStruct();

int foo(int* val) {
    auto paramA = someUndefinedRetStruct();
    for(;true;){
        if (*paramA->a == 123) {  //||1.1.32.259
            *val = 1;             //|0.0.16.0|   |0.0.16.2|
            return 0;
        }
    }
}

int main()
{
    int a;
    auto ret = foo(&a);//0.0.31.0
    if (ret != 0) {
        return -1;
    }
    return a - 1;
}
