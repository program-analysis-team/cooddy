#include <stdlib.h>

#include <cstdint>

struct DDP_VERIFY_BATCH_INFO_S {
    int entrySumInChain;
    int entrySumInSgl;
};

void allocStructure(uint32_t pid, uint32_t partitionID, void **outstructurePtr, const char *funcName,
                    const int32_t fileLine) {
    *outstructurePtr = malloc(0x1234);
}

void SfpAnalyzeAllocInternal(int type, void** structMem)
{
    switch (type) {
        case 5:
            allocStructure(1235, 1234, (structMem), "_function_name_", 357);
            break;
    }
}

void* sfpAnalyzeAlloc(int type)
{
    void* struct_mem = 0;
    SfpAnalyzeAllocInternal(type, (void**)&(struct_mem));
    return struct_mem;
}

int32_t allocFpEvictMem(DDP_VERIFY_BATCH_INFO_S** req)
{
    *req = (DDP_VERIFY_BATCH_INFO_S*)sfpAnalyzeAlloc(5);
    if (*req == NULL) {
        return -1;
    }
    return 1;
}
