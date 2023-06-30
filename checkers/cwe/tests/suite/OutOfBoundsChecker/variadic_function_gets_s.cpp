#include <stdio.h>

void foo()
{
    int buffer[10] = {0};
    char str;
    char* s = gets_s(&str, 1);
    buffer[str];  // error here
    buffer[s[1]];    // error here
}
