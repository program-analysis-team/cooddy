#include <stdlib.h>

void foo(int f)
{
    char* buf = (char*)malloc(42);
    if (f) {
        free(buf);
    }
}
