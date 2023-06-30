#include <stdlib.h>

#include <cstdint>

static int32_t allocArgBuff(char **cfgTexBuf, uint32_t *cfgTexBufLen)  {
    *cfgTexBuf = (char*)malloc(*cfgTexBufLen);
    return 0;
}
void freeBuff(void *objPtr, uint32_t pid, const char *funcName, const int32_t fileLine) {
    free(objPtr);
}

void pmgrAllocateMsgMemory(uint32_t len)
{
    void *val;
    uint32_t cfgTexBufLen = 123;
    allocArgBuff((char **)&val, &cfgTexBufLen);

    if (val != nullptr) {
        *(uint8_t *)val = 1;
        val = (uint8_t *)val + 1;
    }
    freeBuff(val, 1234, "_function_name_", 124);
}
