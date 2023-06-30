#include <cstdint>

void test1()
{
    int arr[UINT8_MAX + 1];
    uint8_t x = UINT8_MAX;  // UINT8_MAX
    int8_t y = x;           // -1
    auto k = arr[y];    // OOB here
}
