#include <stdlib.h>

struct Foo {
    int size;
};

int bar2(int* arr, int to)
{
    int i = 0;
    int v = arr[7];  // no error here
    do {
        arr[i++] = 0;  // error here
    } while (i < to);
    return 0;
}

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK bar(const Foo* foo)
{
    int* arr = (int*)malloc(10 * sizeof(int));
    return bar2(arr, foo->size);
}
