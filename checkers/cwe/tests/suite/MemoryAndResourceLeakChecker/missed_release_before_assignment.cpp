#include <stdlib.h>

char* getBlock()
{
    char* buf = (char*)malloc(42);
    buf = (char*)malloc(100);
    return buf;
}
