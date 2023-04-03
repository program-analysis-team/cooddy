#include <stdlib.h>
#include <cstdint>


int32_t allocBuf(uint32_t structSize, void** outStructPtr, const char* funcName, const int32_t fileLine, uint32_t owner_modid) {
    *outStructPtr = malloc(0x2345);
    return 0;
}
void freeBuff(void * objPtr, uint32_t pid, const char* funcName, const int32_t fileLine) {
    free(objPtr);
}

void mrAsanAllocPubStructure(uint32_t size, void**data, const char *func, const int32_t line)
{
    allocBuf(size, data, func, line, 1234);
}

void *mrAllocPubStructure(uint32_t size, uint8_t initData, bool isCheckLeak, const char *func, const int32_t line)
{
    void *data;
    mrAsanAllocPubStructure(size, (void **)&data, func, line);
    return data;
}

void initFunc()
{
    void* diskIdList = nullptr;
    diskIdList = mrAllocPubStructure(1234, (0xFF), 1, "_function_name_", 435);
    freeBuff(diskIdList, 1234, "_function_name_", 124);
}
