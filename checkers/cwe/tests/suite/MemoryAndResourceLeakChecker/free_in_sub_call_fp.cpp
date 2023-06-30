#include <stdlib.h>

#include <cstdint>

int32_t allocArgBuff(void **cfgTexBuf);
void freeStructure(void *structurePtr);

int someCall();

void *createPoolZoneAllocator()
{
    void *val;
    allocArgBuff(&val);
    if (someCall()) {
        freeStructure(val);
        return nullptr;
    }
    return val;
}

void *createMetaZoneAllocator()
{
    void *metaAllocator;
    metaAllocator = createPoolZoneAllocator();
    return metaAllocator;
}
