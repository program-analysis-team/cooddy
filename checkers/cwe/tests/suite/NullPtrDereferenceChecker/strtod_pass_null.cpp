#include <stdio.h>
#include <string.h>

#include <cstdlib>

double test()
{
    char str[] = "This is a sample string";
    char* pch;
    printf("Looking for the 's' character in \"%s\"...\n", str);
    pch = strchr(str, 'X');
    return strtod(pch, nullptr);
}

int main()
{
    test();
}
