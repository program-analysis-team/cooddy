#include <stdio.h>

void foo()
{
    int arr[10] = {};
    int idx = 0;
    int a = wscanf(L"%d", &idx);
    arr[idx];  // error here
    arr[a];    // error here
}
