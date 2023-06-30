struct A {
    int* ptr;
};

A initFunc() {
    A* arg = new A();
    int val;
    arg->ptr = &val;
    return *arg; //DoNotReturnAddrOfLocalVarChecker
}
