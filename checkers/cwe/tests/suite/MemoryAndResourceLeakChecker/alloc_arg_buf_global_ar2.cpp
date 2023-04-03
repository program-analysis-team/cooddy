#include <stdlib.h>

#include <cstdint>

void allocArgBuff(char** req, uint32_t moduleID, const char* funcName, const int32_t fileLine) {
    *req = (char*)malloc(fileLine);
}

char* g_array[123];

void initFunc()
{
    char* tmpReq;
    allocArgBuff((&tmpReq), 123, "_function_name_", 7243);
    g_array[0] = tmpReq;
    allocArgBuff((&tmpReq), 123, "_function_name_", 7243);
    g_array[2] = tmpReq;
}
