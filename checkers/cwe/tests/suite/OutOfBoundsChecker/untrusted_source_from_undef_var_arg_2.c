#include <stdio.h>

void foo1()
{
    int arr[10] = {};
    int idx = 0;
    printf("%i", idx);
    arr[idx];  // no error here
}

void foo2()
{
    int arr[10] = {};
    int idx = 0;
    printf("%p", &idx);
    arr[idx];  // no error here
}
