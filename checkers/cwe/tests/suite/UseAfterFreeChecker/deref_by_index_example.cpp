#include <iostream>
#include <stdlib.h>

int main ()
{
    int * a;
    a = (int*) malloc (100*sizeof(int));
    free(a);
    a[10] = 12;
    return 0;
}
