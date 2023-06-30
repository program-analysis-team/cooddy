#include <stdlib.h>
struct B {
    int b;
    B* next;
};

struct A {
    B* val;
};

B* getVal(A* param)
{
    if (param == nullptr) {
        return nullptr;
    }
    return param->val;
}

int test(A* param)
{
    B* val = getVal(param);
    val->next = (B*)calloc(1, sizeof(B));  // FP here
    return 0x1234;
}
