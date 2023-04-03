#include <iostream>
#include <stdlib.h>

int main ()
{
    int * a;
    a = (int*) malloc (100*sizeof(int));
    free(a);
    a = nullptr;
    int b = 12;
    b = b + *a;
    return b;
}
