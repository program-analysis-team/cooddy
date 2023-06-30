#include <stdio.h>

void foo()
{
    int arr[10] = {};
    int idx;
    size_t ret = fread(&idx, sizeof(int), 1, fopen("somefile.dat", "r"));
    arr[idx];  // error here
    arr[ret + 8];  // no error here
    arr[ret + 9];  // error here
}
