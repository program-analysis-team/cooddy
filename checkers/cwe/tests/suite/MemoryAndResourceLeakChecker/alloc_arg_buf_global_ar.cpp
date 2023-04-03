#include <stdlib.h>

#include <cstdint>

void allocArgBuff(char** req, uint32_t moduleID, const char* funcName, const int32_t fileLine) {
    *req = (char*)malloc(fileLine);
}

struct A {
    char* ptr;
};

extern A g_array[123];

void initFunc()
{
    A* tmpReq;
    for (int i = 0; i < 123; i++) {
        tmpReq = &(g_array[i]);
        allocArgBuff(&(tmpReq->ptr), 123, "_function_name_", 7243);
    }
}
