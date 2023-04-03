#include <stdint.h>

int memcpy_s(void* dstBuff, int dstMax, const void* srcBuff, int srcSize);

#define SRC_STR "Some data"

typedef struct A {
    uint8_t sfpPrefix[5];
    uint8_t sfp[6];
    uint8_t encodedData;
    uint64_t lbaId;
    uint8_t fp[3];
    uint8_t gradFeatures[7];
} STASH_OP_KEY_S;

int main()
{
    STASH_OP_KEY_S dstBuff;
    memcpy_s(&dstBuff, sizeof(STASH_OP_KEY_S), SRC_STR, sizeof(SRC_STR));
    return 0;
}
