#include <stdio.h>
#include <stdlib.h>

char test(long lSize, FILE* pFile, char* buffer)
{
    fread(buffer, 1, lSize, pFile);
    return buffer[1];
}
