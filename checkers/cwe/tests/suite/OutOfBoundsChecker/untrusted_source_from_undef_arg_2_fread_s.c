#include <stdio.h>

void foo()
{
    int arr[10] = {};
    int idx;
    size_t ret = fread_s(&idx, sizeof(int), sizeof(int), 1, fopen("somefile.dat", "r"));
    arr[idx];  // error here
    arr[ret];  // error here
}
