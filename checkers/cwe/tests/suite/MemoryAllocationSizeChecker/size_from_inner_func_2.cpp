#include <cstdlib>

void bar1(const char* str, int* x)
{
    *x = atoi(str);
}

void EXTER_ATTACK taint2(char*);

void foo()
{
    char* str;
    taint2(str);
    int idx;
    bar1(str, &idx);
    malloc(idx);
}
