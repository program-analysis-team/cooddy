#include <stdlib.h>

void foo()
{
    void* arr[10];
    for (int i = 0; i < 10; i++) {
        arr[i] = malloc(100);
    }
    for (int i = 0; i < 10; i++) {
        free(arr[i]);
    }
}
