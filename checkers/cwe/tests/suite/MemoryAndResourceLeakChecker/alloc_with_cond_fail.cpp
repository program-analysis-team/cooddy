#include <stdlib.h>

char* getBlock(unsigned f)
{
    char* buf = nullptr;
    if (f) {
        buf = (char*)malloc(f);
    }
    if (buf == nullptr) {
        return nullptr;
    }
    return buf;
}
