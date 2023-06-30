struct A {
    int* ptr;
    A(int* b) {
        ptr = b;
    }
};

A* initFunc() {
    int b;
    A* arg = new A(&b);
    return arg; //DoNotReturnAddrOfLocalVarChecker
}
