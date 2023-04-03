#include <iostream>

int memcpy_s(void* dstBuff, int dstMax, const void* srcBuff, int srcSize);

void copy(void* dst, const void* src)
{
    int srcSize, dstSize;
    std::cin >> srcSize;
    dstSize = srcSize;
    std::cin >> srcSize;

    if (dstSize != srcSize) {
        memcpy_s(dst, dstSize, src, srcSize);
    }
}
