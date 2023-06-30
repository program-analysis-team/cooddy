#include <string.h>
#include <iostream>

void f(FILE* f)
{
    char b[8];
    int a = fread(b, sizeof(float), 8, f);
    memcpy(&b[1], &b[1], a);
}
