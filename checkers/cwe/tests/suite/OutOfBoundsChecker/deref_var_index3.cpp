#include <malloc.h>

int main()
{
    int* arr = (int*)malloc(10 * sizeof(int));
    arr += 5;
    int v = *(arr + 3);  // no error here
    arr += 4;
    return *(arr + 1);  // out of bounds
}
