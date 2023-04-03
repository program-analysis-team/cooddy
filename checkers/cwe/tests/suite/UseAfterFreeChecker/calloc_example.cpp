#include <iostream>
#include <stdlib.h>

int main ()
{
    int * a;
    a = (int*) calloc(100, sizeof(int));
    free(a);
    int b = *a;
    return b;
}
