#include <stdlib.h>

void initFunc(int num, char* var[])
{
    for (int i = 0; i < num; i++) {
        var[i] = (char *)malloc(0x124);
    }
}
