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
    free(bPtr);
    free(aPtr);
    return 0;
}
