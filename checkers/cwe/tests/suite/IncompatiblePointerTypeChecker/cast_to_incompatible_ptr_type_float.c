#include <stdio.h>

void foo()
{
    float f = 0.0f;
    int* ip = (int*)&f;
    (*ip)++;
    printf("float is %f\n", f);
}
