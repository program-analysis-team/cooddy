#include <stdio.h>
#include <stdlib.h>

void foo() {
    int* a;
    a = 0;
    free(a);
}
