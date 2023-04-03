
#include "stdlib.h"

void bar(int* ptr)
{
    free(ptr);
}

int main()
{
    int* a = (int*)malloc(sizeof(int));
    free(a);
    bar(a);
}
