#include <stdint.h>

#define MAX_SIZE 100

uint32_t g_arr[MAX_SIZE], g_arr1[MAX_SIZE];
uint32_t g_arr_size;

void foo(uint32_t v)
{
    uint32_t idx = (v < MAX_SIZE) ? v : MAX_SIZE;
    if (idx < g_arr_size) {
        g_arr[idx] = 0;  // no error here (assume g_arr_size has MAX_SIZE value)
    }
}
