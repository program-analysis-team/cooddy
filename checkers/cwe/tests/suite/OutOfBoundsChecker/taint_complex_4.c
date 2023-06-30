// Source: output parameter of an unknown function
// Propagation: arithmetic operations
// Path sensitivity: incompatible conditions
// Sink: pointer arithmetic
#include <stddef.h>

void taint(int* ptr);

void f(int cond)
{
    int arr[10] = {};

    int val = NULL;
    taint(&val);

    int x = 1;

    if (cond) {
        x += val;
    }

    if (!cond) {
        *(arr + x) = 0;
    }
}
