#include <malloc.h>

#define SIZE 8

int main()
{
    int* aPtr;
    int* bPtr = malloc(SIZE * sizeof(int));
    if (bPtr == 0) {
        return -1;
    }
    aPtr = bPtr;
    *aPtr = 0;
    aPtr++;
    aPtr = bPtr;
    free(bPtr);  // Double free FP
    return 0;
}
