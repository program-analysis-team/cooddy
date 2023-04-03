#include <stdlib.h>

#include <cstdint>

int32_t allocArgBuff(void **cfgTexBuf);
void freeStructure(void *structurePtr);

void *arr[10];

void *myAlloc(uint32_t idx)
{
    allocArgBuff(&arr[idx]);
    return arr[idx];
}
