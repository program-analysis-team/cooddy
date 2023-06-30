#include <stdio.h>

void foo()
{
    int arr[10] = {};
    int idx = 0;
    int a = scanf("%d", &idx);
    arr[idx];  // error here
    arr[a];    // error here
}
