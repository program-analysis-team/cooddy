#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstdint>

struct A {
    int b;
    char* a;
};
int32_t poolAllocCfgTextBuf(char** cfgTexBuf, uint32_t* cfgTexBufLen);

int test()
{
    A* a;
    uint32_t cfgTexBufLen = 123;
    poolAllocCfgTextBuf((char**)&a, &cfgTexBufLen);
    a->b = 234;
    auto var =  a->b;
    return var;
}
