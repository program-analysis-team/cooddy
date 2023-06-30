#include <stdio.h>
#include <stdlib.h>

void test(int* buf1 = 0)
{
    buf1 = (int *)calloc(25, sizeof(int));
    *buf1 = 123;
    int val = *buf1;
}

int main() {
    test();
    return 0;
}
