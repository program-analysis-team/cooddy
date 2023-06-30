#include <cstdlib>

const char* EXTER_ATTACK taint2();
int bar2(const char*);

void bar1(const char* str, int* a)
{
    *a = bar2(str);
}

void foo()
{
    int sz;
    auto str = taint2();
    bar1(str, &sz);
    malloc(sz);
}
