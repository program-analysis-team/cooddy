#include <iostream>
#include <stdlib.h>

int main ()
{
    int * a, * c;
    //int * c;
    a = (int*) malloc (100*sizeof(int));
    c = (int*) malloc (1*sizeof(int));
    free(a);
    int b = 12;
    b = b + *c;
    return b;
}
