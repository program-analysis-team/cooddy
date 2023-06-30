#include <cstdlib>

void foo1()
{
    int *x = (int *)malloc(10);
    if (x == 0) {
        return;
    }
    free(x);
}
