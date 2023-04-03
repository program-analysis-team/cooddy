#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

struct A{};
struct SGL_S {
    A* entrySumInChain;
    int entrySumInSgl;
};

void allocSglChain(A** sglChain) {
    *sglChain = (A*)malloc(0x1234);
}
void foo(A*);

void foo1(A* arg) {
    *arg = {};
    free(arg);
}

void allocAndInitSglCom(SGL_S* sglChain)
{
    allocSglChain(&(sglChain->entrySumInChain));
    foo1(sglChain->entrySumInChain);
    return;
}
