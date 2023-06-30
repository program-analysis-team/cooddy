#include <stdlib.h>

int bar(int *x)
{
    free(x); // FP here
    throw 0;
}

int main()
{
    int* x = (int*)malloc(sizeof(int));
    bar(x);
    return *x;
}
