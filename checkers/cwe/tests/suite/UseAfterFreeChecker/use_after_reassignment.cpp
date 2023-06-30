#include <stdlib.h>

typedef struct {
    int* p;
} S;

int foo(S* s1, S* s2, int v)
{
    if (v == 1) {
        free(s1->p);
    }
    *s1 = *s2;
    return *s1->p;  // no error here
}
