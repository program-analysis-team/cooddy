#include <stdlib.h>

#include <cstdint>

void allocArgBuff(char** req, uint32_t moduleID, const char* funcName, const int32_t fileLine) {
    *req = (char*)malloc(fileLine);
}

struct A {
    char* ptr;
};

A* g_a;

void initFunc()
{
    A* tmpReq;
    tmpReq = g_a;
    allocArgBuff(&(tmpReq->ptr), 123, "_function_name_", 7243);
}
