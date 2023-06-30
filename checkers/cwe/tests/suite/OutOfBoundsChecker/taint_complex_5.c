// Source: structure output parameter
// Propagation: copy function
// Path sensitivity: condition on the path
// Sink: array index
#include <string.h>

struct X {
    int a;
    int b;
    int c;
};

void taint(struct X* ptr);

void f(int cond)
{
    int arr[10] = {};

    struct X val = {};
    taint(&val);

    struct X val2 = {};
    memcpy(&val2, &val, sizeof(struct X));

    if (cond) {
        arr[val2.c] = 0;
    }
}
