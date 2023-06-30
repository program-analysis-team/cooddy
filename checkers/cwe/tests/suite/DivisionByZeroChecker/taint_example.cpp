#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int a, b, c, d;
    char input[256];
    a = atoi(fgets(input, sizeof(input), stdin));  // source
    b = a - 10;
    c = 0 / b;   // sink 1
    d = 10 % c;  // sink 2
    return b + c;
}
