#include <iostream>
#include <stdlib.h>

int foo ()
{
    int * a;
    a = (int*) malloc (100*sizeof(int));
    int b = *a;
    free(a);
    return b;
}

int main ()
{
    int * a;
    a = (int*) malloc (100*sizeof(int));
    int b = 12;
    b = b + *a;
    return b;
}
