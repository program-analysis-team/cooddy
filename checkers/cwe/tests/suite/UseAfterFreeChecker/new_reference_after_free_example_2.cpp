#include <stdlib.h>

int main ()
{
    void * a;
    a = malloc(100*sizeof(int));
    free(a);
    a = malloc(50*sizeof(int));
    int b = 12;
    b = b + *(int*)a;
    return b;
}
