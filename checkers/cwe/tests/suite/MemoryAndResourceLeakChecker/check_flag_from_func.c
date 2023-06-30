#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

int32_t allocBuffArg(uint32_t structSize, void **outStructPtr, const char *funcName, const int32_t fileLine, uint32_t owner_modid) {
    *outStructPtr = malloc(0x1234);
    return 0;
}

void *allocBuff(uint32_t size, uint8_t initData, bool isCheckLeak, const char *func, const int32_t line)
{
    void *data = 0;
    int32_t ret = 0;
    ret = allocBuffArg(size, (void **)&data, func, line, 123);
    if (0 != ret || 0 == data) {
        return 0;
    }

    return data;
}
