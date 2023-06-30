#include <stdlib.h>

char* getBlock(int f)
{
    char* buf = (char*)malloc(42);
    if (buf == nullptr) {
        return nullptr;
    }
    return buf;
}
