#include <stdlib.h>

#include <cstdint>

void allocArgBuff(uint32_t pid, uint32_t partitionID, void** outstructurePtr, const char* funcName,
                    const int32_t fileLine)
{
    *outstructurePtr = malloc(0x1234);
}

struct A {
    char* ptr[5];
};

A* getSomeA();

A* initFunc()
{
    A* data = new A();
    allocArgBuff(123, 123, ((void**)&(data->ptr)), "_function_name_", 4586);

    return data;
}
