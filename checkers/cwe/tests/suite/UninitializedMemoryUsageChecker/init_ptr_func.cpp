#include <stdio.h>
#include <stdlib.h>

void init(int **buf)
{
    *buf = (int *)calloc(25, sizeof(int));
}

void test()
{
    int *buf1;
    init(&buf1);
    *buf1 = 123;
    int val = *buf1;
}
