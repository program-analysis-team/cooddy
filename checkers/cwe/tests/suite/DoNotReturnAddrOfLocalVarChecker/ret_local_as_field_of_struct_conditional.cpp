struct A {
    int* ptr;
};

A* initFunc(int c)
{
    A* arg = new A();
    int val;
    if (c > 0) {
        arg->ptr = &val; // DoNotReturnAddrOfLocalVarChecker
    }
    if (c > 5) {
        return arg;
    }
    return nullptr;
}
