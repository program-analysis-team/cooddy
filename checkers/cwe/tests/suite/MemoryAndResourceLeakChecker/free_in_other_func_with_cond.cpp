#include <cstdint>
#include <stdlib.h>

void freeBuff(void *objPtr, uint32_t pid, const char *funcName,
                            const int32_t fileLine) {
    free(objPtr);
}
void mrFreePubStructure(void **mem) {
    if (0 == mem) {
        return;
    }

    freeBuff(*mem, 1, 0, 1);
    return;
}

static int32_t mrShowLostCntHandlerCli() {
    uint64_t *recordCnt = (uint64_t *)malloc(0x1234);
    mrFreePubStructure((void **)&recordCnt);

    return 0;
}
