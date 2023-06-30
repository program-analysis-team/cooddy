#include <cstdlib>

int bar1(const char* str)
{
    int x = atoi(str);
    return x;
}

void EXTER_ATTACK taint(char*);

void foo()
{
    char* str;
    taint(str);
    auto idx = bar1(str);
    malloc(idx);
}
