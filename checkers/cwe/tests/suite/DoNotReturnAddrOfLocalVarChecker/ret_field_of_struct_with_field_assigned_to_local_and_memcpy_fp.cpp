#include <cstring>

struct A {
    int* ptr;
    int code;
};

int initFunc(int* pMem) {
    A* arg = new A();
    int val;
    arg->ptr = &val;
    arg->code = val > 5 ? 1 : 0;
    std::memcpy(pMem, &val, sizeof(val));
    return arg->code; //DoNotReturnAddrOfLocalVarChecker
}
