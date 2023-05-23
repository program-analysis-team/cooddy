#include <stdlib.h>

struct S {
    int field;
};

void* my_malloc(int size)
{
    if (size == 0) {
        return 0;
    }
    return malloc(size);
}

void foo(void** p)
{
    S* s = (S*)my_malloc(sizeof(S));
    *p = &s->field;
}