#include <stdlib.h>

#include <cstdint>
extern void* allocBuf(uint32_t structSize,  const char* funcName, const int32_t fileLine){
    return malloc(0x1234);
}

extern void freeBuf(void * objPtr){
    free(objPtr);
}

int test()
{
    uint32_t valueLen = 123;
    void *valueBuf = allocBuf(1234, "_function_name_", 419);
    freeBuf((valueBuf));
}

int main()
{
    test();
}
