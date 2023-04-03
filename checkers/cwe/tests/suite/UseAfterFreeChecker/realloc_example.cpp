#include <iostream>
#include <stdlib.h>

int main ()
{
    int * a;
    a = (int*) realloc(a, 15*sizeof(int));
    free(a);
    int b = *a;
    return b;
}
