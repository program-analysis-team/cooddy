#include <cstdint>
void test_bad_01(uint8_t* pValue, int uLen)
{
    uint8_t* pOut;
    uint8_t* ptr;
    ptr = pValue + 1;
    while (uLen > 3) {
        uLen--;
        *ptr++ = 1;
        *ptr++ = 2;
    }
}
