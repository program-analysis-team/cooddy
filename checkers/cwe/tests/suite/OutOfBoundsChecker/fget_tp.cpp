#include <stdio.h>
int test(FILE* pFile)
{
    int c = fgetc(pFile);
    int arr[1234];
    arr[c] = 987;
    return 0;
}
