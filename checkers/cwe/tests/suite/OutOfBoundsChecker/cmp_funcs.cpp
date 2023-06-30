#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int test()
{
    char* input = getenv("EXE_PATH");
    int index = strcmp(input,"str2");
    int arr[14];
    arr[index] = 987;
    index = strcasecmp(input,"str2");
    arr[index] = 5678;
    index = strncasecmp(input,"str2", 2);
    arr[index] = 1;

    return 0;
}
