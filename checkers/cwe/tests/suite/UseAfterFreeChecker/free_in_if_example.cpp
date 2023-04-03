#include <iostream>
#include <stdlib.h>

int main ()
{
    int * a;
    a = (int*) malloc (100*sizeof(int));
    if (*a > 0){
        free(a);
    }
    int b = 12;
    b = b + *a;
    return b;
}
