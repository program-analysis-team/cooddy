#include <stdlib.h>

#include <cstdint>

static int32_t allocArgBuff(char **cfgTexBuf) {
    *cfgTexBuf = (char*)malloc(0x1234);
    return 0;
}

void *pmgrAllocateMsgMemory()
{
    void *val;
    allocArgBuff((char **)&val);

    if (val != nullptr) {
        *(uint8_t *)val = 1;
        val = (uint8_t *)val + 1;
    }
    return val;
}

void pmgrFreeMsgMemory(void *ptr)
{
    if (ptr == nullptr) {
        return;
    }
    ptr = (uint8_t *)ptr - 1;
    if (*(uint8_t *)ptr == 1) {
        free(ptr);
    }
}

void initFunc()
{
    auto mem = pmgrAllocateMsgMemory();
    if (mem == nullptr) {
        return;
    }
    pmgrFreeMsgMemory(mem);
}
