#include <stdio.h>

void foo()
{
    int buffer[10] = {0};
    char str[10];
    int s = getw(fopen("file", "r"));
    buffer[s];    // error here
}
