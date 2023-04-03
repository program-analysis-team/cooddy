#include <iostream>
#include <stdlib.h>

int main ()
{
    int * a;
    int b = 12;
    a = (int*) malloc (100*sizeof(int));

    if (*a > 0){
        b = b + *a;
    } else {
        free(a);
    }

    return b;
}
