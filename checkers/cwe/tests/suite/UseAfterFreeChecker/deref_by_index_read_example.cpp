#include <iostream>
#include <stdlib.h>

int main ()
{
    int * a;
    a = (int*) malloc (100*sizeof(int));
    free(a);
    int b = a[44];
    return b;
}
