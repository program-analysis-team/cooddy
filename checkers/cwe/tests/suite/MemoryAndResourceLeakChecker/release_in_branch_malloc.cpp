#include <stdlib.h>

char* getBlock(int f)
{
    char* buf = (char*)malloc(42);
    if (f) {
        free(buf);
        return nullptr;
    }
    return buf;
}
