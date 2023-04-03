#include <cstdint>
struct A{int* ptr;};
struct SGL_S {
    A* entrySumInChain;
    int entrySumInSgl;
};

void allocSglChain(A** sglChain, uint32_t pageCnt, uint32_t moduleID, const char* funcName, const int32_t fileLine);

void foo(A*);

void foo1(A* arg) {
    A* local = arg;
}

void allocAndInitSglCom(SGL_S* sglChain)
{
    allocSglChain(&(sglChain->entrySumInChain), 1234, 1234, "", 123);
    foo1(sglChain->entrySumInChain);
    return;
}
