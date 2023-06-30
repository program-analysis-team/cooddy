#include <cstdint>
#include <cstdlib>

void* foo1(int8_t sz)
{
    if (sz >= 0 && sz < UINT8_MAX) {
        return malloc(sz);  // no detect here
    }
    return nullptr;
}

void* foo2(int8_t sz)
{
    return malloc(sz);  // detect here
}

void test2()
{
    uint8_t x = UINT8_MAX;  // UINT8_MAX
    auto ptr1 = foo1(x);
    free(ptr1);
    auto ptr2 = foo2(x);
    free(ptr2);
}
