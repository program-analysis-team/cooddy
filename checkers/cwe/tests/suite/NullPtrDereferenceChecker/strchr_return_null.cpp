#include <stdio.h>
#include <string.h>

char test()
{
    char str[] = "This is a sample string";
    char* pch;
    printf("Looking for the 's' character in \"%s\"...\n", str);
    pch = strchr(str, 'X');
    return *pch;
}

int main()
{
    test();
}
