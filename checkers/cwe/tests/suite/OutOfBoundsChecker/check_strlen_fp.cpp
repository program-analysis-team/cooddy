#include <stdio.h>
#include <string.h>  // memcpy

void strncpy_sm(char dest[], int destMax, char src[])
{
    if (destMax < strlen(src)) {
        memcpy(dest, src, destMax);
        *((char*)dest + destMax) = '\0';  // no error here
    }
}

int main()
{
    char capType[32];
    strncpy_sm(capType, sizeof(capType), "global");
}
