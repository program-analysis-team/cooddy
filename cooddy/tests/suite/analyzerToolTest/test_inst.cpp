#include <stdio.h>
#include <stdbool.h>

void fillArray(int* arr, size_t size)
{

    if (size <= 0 || size >= 10) {
        return;
    }

    int i = 0, s = 1;

    while (true) {
        if (i >= size) {
            break;
        }

        arr[i] = 1;
        arr[s] = 2;

        i += 2;
        s += 2;
    }
}

#ifndef BOUND
#define BOUND 9
#endif

int main(void)
{
    size_t n = BOUND;
    int arr[BOUND];

    fillArray(arr, n);
    for (int i = 0; i < BOUND; ++i) {
        printf("%d: %d\n", i, arr[i]);
    }

    return 0;
}
