#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

void foo()
{
    int arr[10] = {};
    wchar_t idx;
    char* c = fgetws(&idx, sizeof(int), fopen("somefile.dat", "r"));
    arr[idx];  // error here
    arr[*c];   // error here
}
