#include <stdlib.h>
#include <cstdint>

void freeStructureToPubPool(void* objPtr, uint32_t pid, const char* funcName, const int32_t fileLine);
int32_t allocBuff(uint32_t structSize, void** outStructPtr, const char* funcName,
                                  const int32_t fileLine, uint32_t owner_modid) {
    *outStructPtr = malloc(0x123);
    return 0;
}

int32_t mrAsanAllocPubStructure(uint32_t size, void** data, const char* func, const int32_t line)
{
    return allocBuff(size, data, func, line, 0);  // ret;
}

void* mrAllocPubStructure(uint32_t size, uint8_t initData, bool isCheckLeak, const char* func, const int32_t line)
{
    void* data = NULL;
    int32_t ret = 0;

    ret = mrAsanAllocPubStructure(size, (void**)&data, func, line);
    if (0 != ret || NULL == data) {
        return NULL;
    }

    return data;
}

int dplog(const int mod_id, const int log_level, char* file_name, const int f_line, const char* func_name,
          const long log_id, const char* format, ...);

int32_t mrDestoryMemHandleInner(void* arg)
{
    (void)dplog((const int)1, 0, (char*)0, 0, 0, 0, 0, arg);
    return 0;
}

int someCond(void* arg)
{
    mrDestoryMemHandleInner(arg);
    return -1;
}

void* mrShowLostCntHandlerCli()
{
    void* memHandle = 0;
    memHandle = mrAllocPubStructure(42, 0, 0, 0, 0);

    if (someCond(memHandle)) {
        if (memHandle) {
            return memHandle;
        }
        return NULL;
    }

    return memHandle;
}
