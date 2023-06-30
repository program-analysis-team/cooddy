#include <string.h>
void foo()
{
    int arr[10] = {};
    int idx1 = 0;
    int idx2 = 0;

    memcpy(&idx2, &idx1, sizeof(int));
    arr[idx2] = 0; // no error here
}
