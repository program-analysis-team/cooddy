#include <stdlib.h>

char* getBlock()
{
    char* buf = (char*)malloc(42);
    buf = 0;
    return buf;
}
