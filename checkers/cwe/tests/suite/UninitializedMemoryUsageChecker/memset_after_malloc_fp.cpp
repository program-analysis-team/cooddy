#include <stdlib.h>
#include <string.h>

struct S {
    int p;
};

void undef(S* s);

int foo(bool flag)
{
    auto s = (S*)malloc(sizeof(S));
    if (flag) {
        undef(s);
    } else {
        s->p = 0;
    }
    return s->p;
}
