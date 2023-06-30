#include <stdlib.h>

struct B {
    int* ptr;
};
struct A {
    B* base[123];
};

A* initFunc()
{
    A sid;
    int i = 0;
    sid.base[i]->ptr = (int*)malloc(0x1234);
    free(sid.base[i]->ptr);

    return 0;
}
