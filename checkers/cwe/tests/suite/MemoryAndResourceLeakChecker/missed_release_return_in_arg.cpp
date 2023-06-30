#include <stdlib.h>

void getBlock(int f, char** buf)
{
    char* tempBuf = (char*)malloc(42);
    if (f) {
        *buf = tempBuf;
    }
}
