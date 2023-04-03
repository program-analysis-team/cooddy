#include <malloc.h>

int main()
{
    int* arr = (int*)malloc(10 * sizeof(int));
    int v = arr[9];  // no error here
    return arr[10];  // out of bounds
}
