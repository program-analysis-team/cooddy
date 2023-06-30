#include <stdlib.h>

void getBlock()
{
    char* buf = (char*)malloc(42);
    if (buf == nullptr)
        buf = (char*)malloc(100);
    else
        free(buf);
    return;
}
