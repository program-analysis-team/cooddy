#include <stdio.h>
#include <stdlib.h>

void test()
{
    int *buf1 = (int *)calloc(25, sizeof(int));
    *buf1 = 123;
    int val = *buf1;
}
