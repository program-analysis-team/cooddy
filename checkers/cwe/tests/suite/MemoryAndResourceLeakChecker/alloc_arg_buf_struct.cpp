#include <stdlib.h>

#include <cstdint>

struct A {
    A* ptrA;
    char** ptr;
};

void allocArgBuff(char** req, uint32_t moduleID, const char* funcName, const int32_t fileLine) {
    *req = (char*)malloc(fileLine);
}

void initFunc(A* ctx)
{
    A* io;
    io = ctx;
    char* tmpReq = nullptr;
    allocArgBuff((&tmpReq), 123, "_function_name_", 7243);
    *io->ptr = tmpReq;
}
