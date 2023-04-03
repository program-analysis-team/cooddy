#include <stdlib.h>

#include <cstdint>

void allocArgBuff(uint32_t pid, uint32_t partitionID, void** outstructurePtr, const char* funcName,
                    const int32_t fileLine)
{
    *outstructurePtr = malloc(0x1234);
}

void* initFunc()
{
    void* dataStatistic = nullptr;
    allocArgBuff(123, 123, ((void**)&dataStatistic), "_function_name_", 4586);

    return dataStatistic;
}
