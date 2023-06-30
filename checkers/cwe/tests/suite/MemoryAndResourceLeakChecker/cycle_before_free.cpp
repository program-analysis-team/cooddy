#include <stdlib.h>
#include <cstdint>

bool someFlag();
int32_t allocBuff(uint32_t structSize, void** outStructPtr, const char* funcName, const int32_t fileLine, uint32_t owner_modid) {
    *outStructPtr = malloc(0x1234);
    return 1;
}
void freeBuf(void * objPtr, uint32_t pid, const char* funcName, const int32_t fileLine) {
    free(objPtr);
}

void initFunc(int num, char *var[])
{
    void *diskPoolIdList =0;
    uint32_t dpNum = 0;
    allocBuff(1234, ((void **)&diskPoolIdList), "_function_name_", 496, 123);

    uint32_t dpIdx = 0;
    for (dpIdx = 0; dpIdx < 0x987; dpIdx++) {
    }

    freeBuf((diskPoolIdList), 123, "_function_name_", 512);
}
