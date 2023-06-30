struct A {
    int* ptr;
    int code;
};

int initFunc() {
    A* arg = new A();
    int val;
    arg->ptr = &val;
    arg->code = val > 5 ? 1 : 0;
    return arg->code; //DoNotReturnAddrOfLocalVarChecker
}
