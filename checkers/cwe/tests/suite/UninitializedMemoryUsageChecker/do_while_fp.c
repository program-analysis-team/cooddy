#include <stdint.h>

int* get_ret(int* arr, int idx, uint32_t* bufLen)
{
    int curIndex = idx;
    do {
        if (curIndex < arr[idx]) {
            *bufLen = curIndex;
            return &arr[curIndex];
        }
        curIndex = arr[idx];
    } while (curIndex != 0);
    return 0;
}

int foo(int* arr, int idx)
{
    uint32_t buflen;
    int* ret = get_ret(arr, idx, &buflen);
    if (ret != 0) {
        return buflen;  // no error here
    }
    return 0;
}
