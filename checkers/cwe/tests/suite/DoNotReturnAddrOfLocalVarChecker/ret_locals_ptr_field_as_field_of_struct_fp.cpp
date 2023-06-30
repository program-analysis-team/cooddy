struct A {
    int* ptr;
};

struct B {
    int* x = 0;
    int y = 0;
};

A* initFunc() {
    A* arg = new A();
    B val;
    arg->ptr = val.x;
    return arg; //DoNotReturnAddrOfLocalVarChecker
}
