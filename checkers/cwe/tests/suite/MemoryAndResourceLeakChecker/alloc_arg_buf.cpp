#include <stdlib.h>
#include <cstdint>

static int32_t allocArgBuff(char **cfgTexBuf, uint32_t *cfgTexBufLen)  {
    *cfgTexBuf = (char*)malloc(*cfgTexBufLen);
    return 0;
}

void initFunc(int ** val) {
    uint32_t cfgTexBufLen = 123;
    allocArgBuff((char **)val, &cfgTexBufLen);
}
