#include <stdlib.h>

struct S {
    void* field;
};

S* get_s(int v)
{
    static S arr[10];
    return &arr[v];
}

void foo(int v)
{
    void* p = malloc(10);
    get_s(v)->field = p;
}
