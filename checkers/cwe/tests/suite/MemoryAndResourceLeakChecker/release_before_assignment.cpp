#include <stdlib.h>

char* getBlock(int f)
{
    char* buf = (char*)malloc(42);
    free(buf);
    buf = (char*)malloc(100);
    return buf;
}
