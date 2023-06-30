#include <stdlib.h>

void foo(int *&p)
{
    free(p);
    p = (int *)malloc(sizeof(int));
}

int main()
{
    int *p = (int *)malloc(sizeof(int));
    foo(p);
    int x = *p;
    free(p);
    return x;
}
