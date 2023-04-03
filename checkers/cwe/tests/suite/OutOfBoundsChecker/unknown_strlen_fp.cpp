#include <string.h>

int FP(char* c)
{
    int len = strlen(c);
    for (int i = 0; i < len; i++)
        c[i];  // FP here
    return 0;
}
