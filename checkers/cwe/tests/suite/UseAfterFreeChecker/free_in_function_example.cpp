#include <iostream>
#include <stdlib.h>

int foo (int * b){
    free(b);
    return 0;
}

int main ()
{
    int * a;
    a = (int*) malloc (100*sizeof(int));

    int b = foo(a);
    b = b + *a;
    return b;
}
