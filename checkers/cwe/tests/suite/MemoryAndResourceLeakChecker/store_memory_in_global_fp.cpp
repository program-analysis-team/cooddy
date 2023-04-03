
#include <stdlib.h>

#include <cstdint>

static int32_t allocArgBuff(void **cfgTexBuf) {
    *cfgTexBuf = malloc(0x123);
    return 0;
}
void freeStructure(void *structurePtr);

void *g_arr[10];

void *createPoolZoneAllocator(int idx)
{
    void *val = g_arr[idx];
    if (val == nullptr) {
        allocArgBuff(&val);
        g_arr[idx] = val;
    }
    return val;
}

void *createMetaZoneAllocator(int idx)
{
    void *metaAllocator;
    metaAllocator = createPoolZoneAllocator(idx);
    return metaAllocator;
}
